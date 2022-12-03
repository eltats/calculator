#include "test.h"

int main(void) {
  Suite *testcase[] = {suite_s21_rpn(), suite_s21_calculating(),
                       suite_s21_bank(), NULL};
  for (int i = 0; testcase[i]; i++) {
    printf("\n");
    SRunner *sr = srunner_create(testcase[i]);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);
    srunner_free(sr);
  }
  return 0;
}
