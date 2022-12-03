#include "main.h"

Bank credit(double sum, int term, double interest, bool type) {
  Bank bank = {};
  if (type == ANNUITY) {
    double month_int = interest / 12 / 100, ann_ratio = 0;
    ann_ratio = month_int * powl((1 + month_int), term) /
                (powl((1 + month_int), term) - 1);
    bank.res1[0] = ann_ratio * sum;
    bank.res1[1] = ann_ratio * sum;
    bank.res2 = (bank.res1[0] * term) - sum;
    bank.res3 = bank.res1[0] * term;
  } else {
    double main_payment = 0;
    double remaining = 0;
    double month_payment = 0;
    for (int i = 0; i < term; i++) {
      main_payment = sum / term;
      remaining = (sum - (main_payment * i)) * ((interest / 100) / 12);
      month_payment = main_payment + remaining;
      if (i == 0) bank.res1[0] = month_payment;
      bank.res3 += month_payment;
    }
    bank.res2 = bank.res3 - sum;
    bank.res1[1] = month_payment;
  }
  return bank;
}
Bank deposit(long double sum, unsigned int term, long double interest,
             long double taxes, bool periodicity, bool capitalization,
             long double replenishments, long double withdraw) {
  long double earnings = 0;
  Bank bank = {};
  int period = periodicity == MONTHLY ? 12 : 1;
  withdraw /= period;
  replenishments /= period;
  for (int i = 0; i < term; i++) {
    earnings = sum * interest / 100 / period;
    sum += replenishments;
    sum -= withdraw;
    if (capitalization) sum += earnings;
    bank.res1[0] += earnings;
  }
  if (taxes != 0)
    bank.res3 = sum + (bank.res1[0] * taxes / 100);
  else
    bank.res3 = sum;
  bank.res2 = bank.res1[0] * taxes / 100;
  return bank;
}
