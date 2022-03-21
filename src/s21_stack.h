#ifndef SRC_S21_STACK_H_
#define SRC_S21_STACK_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct last_t {
  char *lastElem;
  struct last_t *secondLastElem;
} Last;

typedef struct stack_t {
  size_t size;
  Last *last;

  void (*Push)(struct stack_t *self, char *elem);
  char *(*Pop)(struct stack_t *self);
  char *(*Peek)(struct stack_t *self);

  void (*Destroy)(struct stack_t **self);
  bool (*IsEmpty)(struct stack_t *self);
} Stack;

Stack *StackInit(void);

void StackPush(Stack *self, char *elem);
char *StackPop(Stack *self);
char *StackPeek(Stack *self);

void StackDestroy(Stack **self);
bool StackIsEmpty(Stack *self);

#endif  // SRC_S21_STACK_H_
