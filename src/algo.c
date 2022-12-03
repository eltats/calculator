#include "main.h"

yard ops[] = {{"+", 5, LEFT},       {"-", 5, LEFT},       {"*", 8, LEFT},
              {"/", 8, LEFT},       {"^", 9, RIGHT},      {"%", 8, LEFT},
              {"~", 10, RIGHT},     {"#", 10, RIGHT},     {"cos", 13, LEFT},
              {"sin", 13, LEFT},    {"tan", 13, LEFT},    {"arccos", 13, LEFT},
              {"arcsin", 13, LEFT}, {"arctan", 13, LEFT}, {"sqrt", 13, LEFT},
              {"ln", 13, LEFT},     {"log", 13, LEFT},    {"(", 0, NONE},
              {")", 0, NONE}};

bool precedence_higher(char *A, stack *operators) {
  assert(A);
  char *last_operator = seek(operators);
  int A_prec = 0, B_prec = 0, B_ass = 0, A_ass = 0;
  int result = 0;
  for (int i = 0; i < OP_SIZE; i++) {
    if (strcmp(ops[i].op, last_operator) == 0) {
      B_prec = ops[i].precedence;
      B_ass = ops[i].associativity;
    }
    if (strcmp(ops[i].op, A) == 0) {
      A_prec = ops[i].precedence;
      A_ass = ops[i].associativity;
    }
  }
  if (B_ass != NONE && A_ass != NONE)
    result = A_prec < B_prec || (A_prec == B_prec && A_ass == LEFT);
  return result;
}

void handling_unary(stack *expression) {
  short is_prev_num = 0;
  for (; expression != NULL; expression = expression->next) {
    if (isoperand(expression->data)) {
      is_prev_num = 1;
    } else if (strcmp(expression->data, "(") != 0 &&
               strcmp(expression->data, ")") != 0) {
      if (strcmp(expression->data, "+") == 0 && is_prev_num == 0)
        strcpy(expression->data, "~");
      if (strcmp(expression->data, "-") == 0 && is_prev_num == 0)
        strcpy(expression->data, "#");
      is_prev_num = 0;
    }
  }
}

stack *convert_to_list(char *expression) {
  char substr[30] = "";
  stack *result = NULL;
  for (int i = 0; expression[i]; i++) {
    if (strspn(&expression[i], OPERANDS) != 0) {
      for (int len = 0;
           expression[i] && strspn(&expression[i], OPERANDS) != 0 && len < 29;
           len++, i++) {
        substr[len] = expression[i];
      }
      push_back(&result, substr);
      memset(substr, 0, 30);
    }
    if (strspn(&expression[i], FUNCTIONS) != 0) {
      for (int len = 0;
           expression[i] && strspn(&expression[i], FUNCTIONS) != 0 && len < 29;
           len++, i++) {
        substr[len] = expression[i];
      }
      push_back(&result, substr);
      memset(substr, 0, 30);
    }
    if (expression[i] && strspn(&expression[i], "()+-*/^%#~") != 0) {
      substr[0] = expression[i];
      push_back(&result, substr);
      memset(substr, 0, 30);
    }
    if (!expression[i]) break;
  }
  return result;
}

stack *convert_to_rpn(char *input) {
  assert(input);
  stack *expression = convert_to_list(input);
  handling_unary(expression);
  stack *out = NULL, *operators = NULL, *tmp = NULL, *mmm = NULL;
  while (expression != NULL) {
    if (isoperand(expression->data)) {
      push_back(&out, expression->data);
    } else if (isfunction(expression->data)) {
      push_back(&operators, expression->data);
    } else if (isoperator(expression->data)) {
      while (operators && isoperator(expression->data) &&
             (precedence_higher(expression->data, operators)) == 1) {
        mmm = pop(&operators);
        push_back(&out, mmm->data);
        free(mmm);
      }
      push_back(&operators, expression->data);
    } else if (strcmp(expression->data, "(") == 0) {
      push_back(&operators, expression->data);
    } else if (strcmp(expression->data, ")") == 0) {
      while (operators && strcmp(seek(operators), "(") != 0) {
        mmm = pop(&operators);
        push_back(&out, mmm->data);
        free(mmm);
      }
      mmm = pop(&operators);
      if (mmm) free(mmm);
      if (operators && isfunction(seek(operators))) {
        mmm = pop(&operators);
        push_back(&out, mmm->data);
        free(mmm);
      }
    }
    tmp = expression->next;
    free(expression);
    expression = tmp;
  }
  while (operators) {
    mmm = pop(&operators);
    push_back(&out, mmm->data);
    free(mmm);
  }
  free_stack(tmp);
  return out;
}

long double calculate(stack *rpn) {
  stack *tmp = NULL, *nums = NULL;
  long double num1 = 0, num2 = 0;
  while (rpn != NULL) {
    if (isdigit(rpn->data[0])) {
      push_back_num(&nums, strtod(rpn->data, NULL));
    } else if (isoperator(rpn->data)) {
      if (strcmp(rpn->data, "+") == 0) {
        num1 = pop_num(&nums);
        num2 = pop_num(&nums);
        push_back_num(&nums, num1 + num2);
      }
      if (strcmp(rpn->data, "*") == 0) {
        num1 = pop_num(&nums);
        num2 = pop_num(&nums);
        push_back_num(&nums, num1 * num2);
      }
      if (strcmp(rpn->data, "/") == 0) {
        num2 = pop_num(&nums);
        num1 = pop_num(&nums);
        push_back_num(&nums, num1 / num2);
      }
      if (strcmp(rpn->data, "-") == 0) {
        num2 = pop_num(&nums);
        num1 = pop_num(&nums);
        push_back_num(&nums, num1 - num2);
      }
      if (strcmp(rpn->data, "%") == 0) {
        num2 = pop_num(&nums);
        num1 = pop_num(&nums);
        push_back_num(&nums, fmod(num1, num2));
      }
      if (strcmp(rpn->data, "#") == 0) {
        num1 = pop_num(&nums);
        push_back_num(&nums, num1 * -1l);
      }
      if (strcmp(rpn->data, "^") == 0) {
        num1 = pop_num(&nums);
        num2 = pop_num(&nums);
        push_back_num(&nums, pow(num2, num1));
      }
    } else if (isfunction(rpn->data)) {
      if (strcmp(rpn->data, "sin") == 0) {
        num1 = pop_num(&nums);
        push_back_num(&nums, sinl(num1));
      }
      if (strcmp(rpn->data, "cos") == 0) {
        num1 = pop_num(&nums);
        push_back_num(&nums, cosl(num1));
      }
      if (strcmp(rpn->data, "tan") == 0) {
        num1 = pop_num(&nums);
        push_back_num(&nums, tanl(num1));
      }
      if (strcmp(rpn->data, "arcsin") == 0) {
        num1 = pop_num(&nums);
        push_back_num(&nums, asinl(num1));
      }
      if (strcmp(rpn->data, "arccos") == 0) {
        num1 = pop_num(&nums);
        push_back_num(&nums, acosl(num1));
      }
      if (strcmp(rpn->data, "arctan") == 0) {
        num1 = pop_num(&nums);
        push_back_num(&nums, atanl(num1));
      }
      if (strcmp(rpn->data, "sqrt") == 0) {
        num1 = pop_num(&nums);
        push_back_num(&nums, sqrtl(num1));
      }
      if (strcmp(rpn->data, "ln") == 0) {
        num1 = pop_num(&nums);
        push_back_num(&nums, logl(num1));
      }
      if (strcmp(rpn->data, "log") == 0) {
        num1 = pop_num(&nums);
        push_back_num(&nums, log10l(num1));
      }
    }
    tmp = rpn->next;
    free(rpn);
    rpn = tmp;
  }
  long double res = 0, res_tmp = 0;
  while ((res_tmp = pop_num(&nums))) {
    res += res_tmp;
  }
  free_stack(nums);
  return res;
}

double solver(char *input) {
  double res = 0;
  if (input && *input) {
    stack *rpn = convert_to_rpn(input);
    assert(rpn);
    res = calculate(rpn);
  }
  return res;
}
