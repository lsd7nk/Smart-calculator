#include "s21_polish.h"

static bool isDigit(char *token) {
  double resultDouble;
  bool resultBool = false;
  int res = sscanf(token, "%lf", &resultDouble);

  if (res == 1) {
    resultBool = true;
  }

  return resultBool;
}

static int GetPriority(char *token) {
  char *mathFunc[9] = {"sqrt", "acos", "asin", "atan", "cos",
                       "sin",  "tan",  "log",  "ln"};
  int priority = 0;

  if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0) {
    priority = 1;
  } else if (strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
             strcmp(token, "%") == 0) {
    priority = 2;
  } else if (strcmp(token, "^") == 0) {
    priority = 3;
  }

  for (int i = 0; i < 9; i++) {
    if (strcmp(token, mathFunc[i]) == 0) {
      priority = 3;
    }
  }

  return priority;
}

static double PerformAnAction(double firstDigit, char *action,
                              double secondDigit) {
  double result;

  if (strcmp(action, "+") == 0) {
    result = firstDigit + secondDigit;
  }
  if (strcmp(action, "-") == 0) {
    result = firstDigit - secondDigit;
  }
  if (strcmp(action, "*") == 0) {
    result = firstDigit * secondDigit;
  }
  if (strcmp(action, "/") == 0) {
    result = firstDigit / secondDigit;
  }
  if (strcmp(action, "%") == 0) {
    result = fmod(firstDigit, secondDigit);
  }
  if (strcmp(action, "^") == 0) {
    result = pow(firstDigit, secondDigit);
  }

  return result;
}

static void Calculate(Stack *operands, Stack *operators) {
  double lastDigit;
  double secondLastDigit;
  char result[256] = {0};

  char* lastDigitPointer = operands->Pop(operands);
  sscanf(lastDigitPointer, "%lf", &lastDigit);

  char* secondLastDigitPointer = operands->Pop(operands);
  sscanf(secondLastDigitPointer, "%lf", &secondLastDigit);

  char *action = operators->Pop(operators);
  double res = PerformAnAction(secondLastDigit, action, lastDigit);

  sprintf(result, "%lf", res);

  char *resPointer = result;
  operands->Push(operands, resPointer);

  free(lastDigitPointer);
  free(secondLastDigitPointer);
  free(action);
}

static void PerformMathFunc(Stack *operands, Stack *operators) {
  double resDouble = 0;
  double digit;
  char *funcName = operators->Pop(operators);
  char *resCharPointer = {0};
  char resChar[256] = {0};

  char *lastDigit = operands->Pop(operands);
  sscanf(lastDigit, "%lf", &digit);

  if (strcmp(funcName, "sqrt") == 0) {
    resDouble = sqrt(digit);
  }
  if (strcmp(funcName, "acos") == 0) {
    resDouble = acos(digit);
  }
  if (strcmp(funcName, "atan") == 0) {
    resDouble = atan(digit);
  }
  if (strcmp(funcName, "asin") == 0) {
    resDouble = asin(digit);
  }
  if (strcmp(funcName, "cos") == 0) {
    resDouble = cos(digit);
  }
  if (strcmp(funcName, "tan") == 0) {
    resDouble = tan(digit);
  }
  if (strcmp(funcName, "sin") == 0) {
    resDouble = sin(digit);
  }
  if (strcmp(funcName, "ln") == 0) {
    resDouble = log(digit);
  }
  if (strcmp(funcName, "log") == 0) {
    resDouble = log10(digit);
  }

  sprintf(resChar, "%lf", resDouble);
  resCharPointer = resChar;

  operands->Push(operands, resCharPointer);

  free(funcName);
  free(lastDigit);
}

static char *GetToken(char *expression, int *position, Stack *operands) {
  char const mathFunc[5] = {'s', 'a', 'c', 't', 'l'};
  char *token = (char *)malloc(sizeof(char) * 256);

  while (true) {
    if ((expression[*position] <= '9' && expression[*position] >= '0') ||
        expression[*position] == '.') {  // digit
      strncat(token, expression + (*position), 1);

      if ((expression[*position + 1] <= '9' &&
           expression[*position + 1] >= '0') ||
          expression[*position + 1] == '.') {
        (*position)++;
      } else {
        (*position)++;

        break;
      }
    } else if (expression[*position] >= 'a' && expression[*position] <= 't') {
      if (expression[*position] == 'a' ||
          (expression[*position] == 's' &&
           expression[*position + 1] == 'q')) {  // acos, atan, asin, sqrt
        strncat(token, expression + (*position), 4);
        (*position) += 4;

        break;
      } else if (expression[*position] == 'l' &&
                 expression[*position + 1] == 'n') {  // ln
        strncat(token, expression + (*position), 2);
        (*position) += 2;

        break;
      } else {  // cos, tan, sin, log
        strncat(token, expression + (*position), 3);
        (*position) += 3;

        break;
      }
    } else if (expression[*position] == '-' ||
               expression[*position] == '+') {  // unary operation (+, -)
      bool isMathFunc = false;

      for (int i = 0; i < 5;
           i++) {  // apply decision (operator is it mathFunc or not)
        if (expression[*position + 1] == mathFunc[i]) {
          isMathFunc = true;
          break;
        }
      }

      if ((expression[*position + 1] <= '9' &&
           expression[*position + 1] >= '0') ||
          isMathFunc) {
        if (!(expression[*position - 1] <= '9' &&
              expression[*position - 1] >= '0')) {
          if (expression[*position - 1] != ')') {
            operands->Push(operands, "0");

            strncat(token, expression + (*position), 1);
            (*position)++;

            break;
          } else {
            strncat(token, expression + (*position), 1);
            (*position)++;

            break;
          }
        } else {
          strncat(token, expression + (*position), 1);
          (*position)++;

          break;
        }
      }

      // -()
      if (expression[*position + 1] == '(' &&
          expression[*position - 1] != ')') {
        if (!(expression[*position - 1] <= '9' &&
              expression[*position - 1] >= '0')) {
          operands->Push(operands, "0");

          strncat(token, expression + (*position), 1);
          (*position)++;

          break;
        }
      }

      strncat(token, expression + (*position), 1);
      (*position)++;

      break;
    } else {  // action
      strncat(token, expression + (*position), 1);
      (*position)++;

      break;
    }
  }

  return token;
}

char *Parser(char *expression) {
  Stack *operators = StackInit();
  Stack *operands = StackInit();

  char *mathFunc[9] = {"sqrt", "acos", "asin", "atan", "cos",
                       "sin",  "tan",  "log",  "ln"};
  int position = 0;

  while (expression[position] != '\0') {
    char *token = GetToken(expression, &position, operands);

    if (isDigit(token)) {
      operands->Push(operands, token);
      free(token);
      continue;
    } else {
      // if get first token || stack is empty
      if (operators->IsEmpty(operators)) {
        operators->Push(operators, token);
        free(token);
        continue;
      }

      // if token == ( || )
      if (strcmp(token, ")") == 0) {
        while (strcmp(operators->Peek(operators), "(") != 0) {
          bool isMathFunc = false;

          for (int i = 0; i < 9;
               i++) {  // apply decision (operator is it mathFunc or not)
            if (strcmp(operators->Peek(operators), mathFunc[i]) == 0) {
              isMathFunc = true;
              break;
            }
          }

          if (isMathFunc) {  // if operator is it mathFunc
            if (operators->IsEmpty(operators) || operands->size < 1) {
              break;
            }

            PerformMathFunc(operands, operators);
          } else {
            if (operators->IsEmpty(operators) || operands->size < 2) {
              break;
            }

            Calculate(operands, operators);
          }
        }

        if (!operators->IsEmpty(operators)) {
          if (strcmp(operators->Peek(operators), "(") == 0) {
            char *bracket = operators->Pop(operators);
            free(bracket);
          }
        }

        free(token);

        continue;
      } else if (strcmp(token, "(") == 0) {
        operators->Push(operators, token);
        free(token);
        continue;
      }

      // Get Priority
      if (GetPriority(token) > GetPriority(operators->Peek(operators))) {
        operators->Push(operators, token);
        free(token);
        continue;
      } else if (GetPriority(token) <=
                 GetPriority(operators->Peek(operators))) {
        while (GetPriority(token) <= GetPriority(operators->Peek(operators)) ||
               strcmp(operators->Peek(operators), "(") == 0) {
          bool isMathFunc = false;

          for (int i = 0; i < 9; i++) {
            if (strcmp(operators->Peek(operators), mathFunc[i]) == 0) {
              isMathFunc = true;
              break;  // apply decision (operator is it mathFunc or not)
            }
          }

          if (isMathFunc) {  // if operator is it mathFunc
            if (operators->IsEmpty(operators) || operands->size < 1) {
              break;
            }

            PerformMathFunc(operands, operators);
          } else {
            if (operators->IsEmpty(operators) || operands->size < 2) {
              break;
            }

            Calculate(operands, operators);
          }

          if (operators->IsEmpty(operators)) {
            break;
          }
        }

        operators->Push(operators, token);
        free(token);
        continue;
      }

      free(token);
    }

    free(token);
  }

  while (!operators->IsEmpty(operators) && operands->size >= 2) {
    Calculate(operands, operators);
  }

  if (!operands->IsEmpty(operands)) {
    char *res = operands->Pop(operands);

    operands->Destroy(&operands);
    operators->Destroy(&operators);

    return res;
  } else {
    operands->Destroy(&operands);
    operators->Destroy(&operators);

    return "error";
  }
}
