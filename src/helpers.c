#include "main.h"

void free_stack(stack *A) {
  stack *next = NULL;
  while (A != NULL) {
    next = A->next;
    free(A);
    A = next;
  }
}
void push_back(stack **A, char *operand) {
  assert(operand);
  stack *new = NULL;
  stack *copy = *A;
  while (copy && copy->next != NULL) {
    copy = copy->next;
  }
  new = (stack *)calloc(1, sizeof(stack));
  CHECKMALLOC(new);
  strcpy(new->data, operand);
  new->next = NULL;
  if (!*A)
    *A = new;
  else
    copy->next = new;
}

void push_back_num(stack **A, long double number) {
  stack *new = NULL;
  stack *copy = *A;
  while (copy && copy->next != NULL) {
    copy = copy->next;
  }
  new = (stack *)calloc(1, sizeof(stack));
  CHECKMALLOC(new);
  new->num = number;
  new->next = NULL;
  if (!*A)
    *A = new;
  else
    copy->next = new;
}

char *seek(stack *A) {
  char *result = NULL;
  if (A) {
    stack *copy = A;
    while (copy && copy->next && copy->next->next) {
      copy = copy->next;
    }
    result = copy->data;
    if (copy->next) result = copy->next->data;
  }
  return result;
}

stack *pop(stack **A) {
  stack *result = NULL;
  stack *copy = (*A);
  while (copy && copy->next && copy->next->next) {
    copy = copy->next;
  }
  if (copy) {
    result = copy;
    if (copy->next) {
      result = copy->next;
      copy->next = NULL;
    } else {
      *A = NULL;
    }
  }
  return result;
}

long double pop_num(stack **A) {
  long double result = 0;
  stack *copy = (*A);
  while (copy && copy->next && copy->next->next) {
    copy = copy->next;
  }
  if (copy) {
    result = copy->num;
    if (copy->next) {
      result = copy->next->num;
      free(copy->next);
      copy->next = NULL;
    } else {
      free(*A);
      *A = NULL;
    }
  }
  return result;
}

bool isfunction(char *A) { return strspn(A, FUNCTIONS) != 0; }

bool isoperator(char *A) { return strspn(A, OPERATORS) != 0; }

bool isoperand(char *A) { return strspn(A, OPERANDS) != 0; }
