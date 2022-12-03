#include "test.h"

START_TEST(mod_unary_plus) {
  char str[] = "+3 % 2";
  long double result = solver(str);
  long double expected = 1;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(mod_unary_minus) {
  char str[] = "3 - -2";
  long double result = solver(str);
  long double expected = 5;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(sin_test) {
  char str[] = "sin(1)";
  long double result = solver(str);
  long double expected = 0.841470;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(tan_test) {
  char str[] = "tan(1)";
  long double result = solver(str);
  long double expected = 1.557407;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(cos_test) {
  char str[] = "cos(1)";
  long double result = solver(str);
  long double expected = 0.540302;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(normal) {
  char str[] = "2 + 5 - 10 * (124 * 152)";
  long double result = solver(str);
  long double expected = -188473;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(mult) {
  char str[] = "9 * 9";
  long double result = solver(str);
  long double expected = 81;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(plus) {
  char str[] = "333 + 333";
  long double result = solver(str);
  long double expected = 666;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(minus) {
  char str[] = "1000 - 334";
  long double result = solver(str);
  long double expected = 666;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(power) {
  char str[] = "2 ^ 3";
  long double result = solver(str);
  long double expected = 8;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(power_more) {
  char str[] = "2 ^ 3 ^ 3";
  long double result = solver(str);
  long double expected = 134217728;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(mod) {
  char str[] = "10 % 2";
  long double result = solver(str);
  long double expected = 0;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(division) {
  char str[] = "10 / 2";
  long double result = solver(str);
  long double expected = 5;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(arcs) {
  char str[] = "arccos(arcsin(arctan(0.8)))";
  long double result = solver(str);
  long double expected = 0.7368131;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(sqrt_log) {
  char str[] = "sqrt(log(100))";
  long double result = solver(str);
  long double expected = 1.4142135;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

START_TEST(sqrt_log10) {
  char str[] = "sqrt(ln(100))";
  long double result = solver(str);
  long double expected = 2.14596602;
  ck_assert_ldouble_eq_tol(result, expected, 1e-06);
}
END_TEST

Suite *suite_s21_calculating() {
  Suite *s = suite_create("suite_s21_calculating");
  TCase *tc = tcase_create("s21_calculating");

  tcase_add_test(tc, normal);

  tcase_add_test(tc, mod_unary_plus);
  tcase_add_test(tc, mod_unary_minus);
  tcase_add_test(tc, arcs);
  tcase_add_test(tc, sqrt_log);
  tcase_add_test(tc, sqrt_log10);
  tcase_add_test(tc, sin_test);
  tcase_add_test(tc, cos_test);
  tcase_add_test(tc, tan_test);

  tcase_add_test(tc, mod);
  tcase_add_test(tc, division);
  tcase_add_test(tc, mod);
  tcase_add_test(tc, mult);
  tcase_add_test(tc, plus);
  tcase_add_test(tc, minus);

  tcase_add_test(tc, power);
  tcase_add_test(tc, power_more);

  suite_add_tcase(s, tc);

  return s;
}