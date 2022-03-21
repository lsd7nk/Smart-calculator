#include "s21_stack.h"

Stack *StackInit(void) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));

  stack->last = NULL;
  stack->size = 0;

  stack->Push = StackPush;
  stack->Pop = StackPop;
  stack->Peek = StackPeek;

  stack->Destroy = StackDestroy;
  stack->IsEmpty = StackIsEmpty;

  return stack;
}

void StackPush(Stack *self, char *elem) {
  Last *last = (Last *)malloc(sizeof(Last));
  asprintf(&last->lastElem, "%s", elem);

  last->secondLastElem = self->last;

  self->last = last;
  self->size++;
}

char *StackPop(Stack *self) {
  char *popElem = NULL;

  if (self->size > 0) {
    Last *last = self->last->secondLastElem;
    char *curLastElem = self->last->lastElem;

    if (curLastElem) {
      popElem = curLastElem;
      // free(self->last->lastElem);
    }
    free(self->last);

    self->last = last;
    self->size--;
  }

  return popElem;
}

char *StackPeek(Stack *self) { return (self->last->lastElem); }

void StackDestroy(Stack **self) {
  Last *last = (*self)->last;
  Last *secondLast = NULL;

  while (last) {
    secondLast = last->secondLastElem;

    if (last->lastElem) {
      free(last->lastElem);
    }
    free(last);

    last = secondLast;
  }

  free(*self);
}

bool StackIsEmpty(Stack *self) { return !(self->size > 0); }
