#pragma once

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OP_SIZE 19
#define OPERATORS "+-*/^%~#"
#define OPERANDS "x1234567890."
#define FUNCTIONS "sincostanacosatanasinsqrtlnlog"
#define CHECKMALLOC(x)                     \
  if ((x) == NULL) {                       \
    fprintf(stderr, "ALLOCATION ERROR\n"); \
    abort();                               \
  }

typedef struct node {
  char data[30];
  long double num;
  struct node *next;
} stack;

typedef struct rslts {
  long double res2, res3;
  long double res1[2];
} Bank;

enum { RIGHT = 0, LEFT, NONE };

enum { MONTHLY, YEARLY };

enum { ANNUITY, DIFF };

typedef struct shunt {
  char op[8];
  short precedence;
  short associativity;
} yard;

stack *convert_to_rpn(char *expression);
void free_stack(stack *A);
double solver(char *input);
Bank deposit(long double sum, unsigned int term, long double interest,
             long double taxes, bool periodicity, bool capitalization,
             long double replenishments, long double withdraw);
Bank credit(double sum, int term, double interest, bool type);
void push_back(stack **A, char *operand);
void push_back_num(stack **A, long double number);
char *seek(stack *A);
stack *pop(stack **A);
long double pop_num(stack **A);
bool isfunction(char *A);
bool isoperator(char *A);
bool isoperand(char *A);
