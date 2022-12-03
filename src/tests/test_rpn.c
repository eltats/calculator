#include "test.h"

START_TEST(simple) {
  char str[] = "2 - 1";
  stack *result = convert_to_rpn(str);
  stack *copy = result;
  char *expected[] = {"2", "1", "-"};
  for (int i = 0; result != NULL && expected[i]; i++, result = result->next) {
    ck_assert_str_eq(result->data, expected[i]);
  }
  free_stack(copy);
}
END_TEST

START_TEST(medium) {
  char str[] = "(5 * 4 + 3 * 2) - 1";
  stack *result = convert_to_rpn(str);
  stack *copy = result;
  char *expected[] = {"5", "4", "*", "3", "2", "*", "+", "1", "-"};
  for (int i = 0; result != NULL && expected[i]; i++, result = result->next) {
    ck_assert_str_eq(result->data, expected[i]);
  }
  free_stack(copy);
}
END_TEST

START_TEST(funcs) {
  char str[] = "sin(x) * 1";
  stack *result = convert_to_rpn(str);
  stack *copy = result;
  char *expected[] = {"x", "sin", "1", "*"};
  for (int i = 0; result != NULL && expected[i]; i++, result = result->next) {
    ck_assert_str_eq(result->data, expected[i]);
  }
  free_stack(copy);
}
END_TEST

START_TEST(pow_test) {
  char str[] = "5^2^3";
  stack *result = convert_to_rpn(str);
  stack *copy = result;
  char *expected[] = {"5", "2", "3", "^", "^"};
  for (int i = 0; result != NULL && expected[i]; i++, result = result->next) {
    ck_assert_str_eq(result->data, expected[i]);
  }
  free_stack(copy);
}
END_TEST

START_TEST(funcs_medium) {
  char str[] = "sin(1)^cos(1)";
  stack *result = convert_to_rpn(str);
  stack *copy = result;
  char *expected[] = {"1", "sin", "1", "cos", "^"};
  for (int i = 0; result != NULL && expected[i]; i++, result = result->next) {
    ck_assert_str_eq(result->data, expected[i]);
  }
  free_stack(copy);
}
END_TEST

START_TEST(multuple_funcs) {
  char str[] = "sin (1) *cos(1)*ln(1)*tan(1)";
  stack *result = convert_to_rpn(str);
  stack *copy = result;
  char *expected[] = {"1",  "sin", "1", "cos", "*", "1",
                      "ln", "*",   "1", "tan", "*"};
  for (int i = 0; result != NULL && expected[i]; i++, result = result->next) {
    ck_assert_str_eq(result->data, expected[i]);
  }
  free_stack(copy);
}
END_TEST

START_TEST(unary) {
  char str[] = "-5 * -3";
  stack *result = convert_to_rpn(str);
  stack *copy = result;
  char *expected[] = {"5", "#", "3", "#", "*"};
  for (int i = 0; result != NULL && expected[i]; i++, result = result->next) {
    ck_assert_str_eq(result->data, expected[i]);
  }
  free_stack(copy);
}
END_TEST

START_TEST(funcs_hard) {
  char str[] =
      "cos(tan( 2.0221 + tan( sin( cos( 5.0945 ) ) - tan(cos(3.6593))  ) "
      "^1.4616 )  -sin(4.1949- sin( cos(tan(tan( cos(3.8475 ) )  ) ) ^  "
      "8.8451) ^  1.4109) -  tan( 2.6884/tan( 5.2519 / 6.5461 ) ))";
  stack *result = convert_to_rpn(str);
  stack *copy = result;
  char *expected[] = {
      "2.0221", "5.0945", "cos", "sin",    "3.6593", "cos",    "tan",    "-",
      "tan",    "1.4616", "^",   "+",      "tan",    "4.1949", "3.8475", "cos",
      "tan",    "tan",    "cos", "8.8451", "^",      "sin",    "1.4109", "^",
      "-",      "sin",    "-",   "2.6884", "5.2519", "6.5461", "/",      "tan",
      "/",      "tan",    "-",   "cos"};
  for (int i = 0; result != NULL && expected[i]; i++, result = result->next) {
    ck_assert_str_eq(result->data, expected[i]);
  }
  free_stack(copy);
}
END_TEST

Suite *suite_s21_rpn() {
  Suite *s = suite_create("suite_s21_rpn");
  TCase *tc = tcase_create("s21_rpn");

  tcase_add_test(tc, simple);
  tcase_add_test(tc, medium);
  tcase_add_test(tc, funcs);
  tcase_add_test(tc, funcs);
  tcase_add_test(tc, multuple_funcs);
  tcase_add_test(tc, pow_test);
  tcase_add_test(tc, funcs_medium);
  tcase_add_test(tc, funcs_hard);
  tcase_add_test(tc, unary);

  suite_add_tcase(s, tc);
  return s;
}