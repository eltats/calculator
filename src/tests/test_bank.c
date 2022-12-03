#include "test.h"

START_TEST(annuity) {
  long double sum = 42069.69;
  unsigned int term = 69;
  long double percent = 4.20;
  Bank bank = credit(sum, term, percent, ANNUITY);
  ck_assert_ldouble_eq_tol(bank.res1[0], 687.349222, 1e-6);
  ck_assert_ldouble_eq_tol(bank.res2, 5357.406315, 1e-6);
  ck_assert_ldouble_eq_tol(bank.res3, 47427.096315, 1e-6);
}
END_TEST

START_TEST(differentiated) {
  long double sum = 42069.69;
  unsigned int term = 69;
  long double percent = 4.20;
  Bank bank = credit(sum, term, percent, DIFF);
  ck_assert_ldouble_eq_tol(bank.res1[0], 756.949567, 1e-6);
  ck_assert_ldouble_eq_tol(bank.res1[1], 611.839622, 1e-6);
  ck_assert_ldouble_eq_tol(bank.res2, 5153.537025, 1e-6);
  ck_assert_ldouble_eq_tol(bank.res3, 47223.227025, 1e-6);
}
END_TEST

START_TEST(deposit_test) {
  long double sum = 69000;
  unsigned int term = 42;
  long double interest = 4.20;
  long double taxes = 0;
  bool periodicity = MONTHLY;
  bool capitalizaton = false;
  long double replen = 0;
  long double withdraw = 0;
  Bank bank = deposit(sum, term, interest, taxes, periodicity, capitalizaton,
                      replen, withdraw);
  ck_assert_ldouble_eq_tol(bank.res1[0], 10143, 1e-0);
  ck_assert_ldouble_eq_tol(bank.res3, 69000, 1e-6);
}

START_TEST(deposit_yearly) {
  long double sum = 69000;
  unsigned int term = 42;
  long double interest = 4.20;
  long double taxes = 0;
  bool periodicity = YEARLY;
  bool capitalizaton = false;
  long double replen = 0;
  long double withdraw = 0;
  Bank bank = deposit(sum, term, interest, taxes, periodicity, capitalizaton,
                      replen, withdraw);
  ck_assert_ldouble_eq_tol(bank.res1[0], 121716, 1e-0);
  ck_assert_ldouble_eq_tol(bank.res3, 69000, 1e-0);
}

START_TEST(deposit_cap) {
  long double sum = 69000;
  unsigned int term = 42;
  long double interest = 4.20;
  long double taxes = 0;
  bool periodicity = MONTHLY;
  bool capitalizaton = true;
  long double replen = 0;
  long double withdraw = 0;
  Bank bank = deposit(sum, term, interest, taxes, periodicity, capitalizaton,
                      replen, withdraw);
  ck_assert_ldouble_eq_tol(bank.res1[0], 10905.912859, 1e-6);
  ck_assert_ldouble_eq_tol(bank.res3, 79905.912859083, 1e-6);
}

START_TEST(deposit_taxes) {
  long double sum = 69000;
  unsigned int term = 42;
  long double interest = 4.20;
  long double taxes = 10;
  bool periodicity = MONTHLY;
  bool capitalizaton = false;
  long double replen = 0;
  long double withdraw = 0;
  Bank bank = deposit(sum, term, interest, taxes, periodicity, capitalizaton,
                      replen, withdraw);
  ck_assert_ldouble_eq_tol(bank.res1[0], 10143, 1e-0);
  ck_assert_ldouble_eq_tol(bank.res3, 70014.3, 1e-1);
  ck_assert_ldouble_eq_tol(bank.res2, 1014.3, 1e-1);
}

Suite *suite_s21_bank() {
  Suite *s = suite_create("suite_s21_bank");
  TCase *tc = tcase_create("s21_bank");

  tcase_add_test(tc, annuity);
  tcase_add_test(tc, differentiated);
  tcase_add_test(tc, deposit_test);
  tcase_add_test(tc, deposit_taxes);
  tcase_add_test(tc, deposit_yearly);
  tcase_add_test(tc, deposit_cap);
  suite_add_tcase(s, tc);
  return s;
}