#ifndef _BOOTSTRAP_TEST_CASE_H
#define _BOOTSTRAP_TEST_CASE_H

struct TestCase {
  char *name;
  int (*func)();
};

#endif /* _BOOTSTRAP_TEST_CASE_H */
