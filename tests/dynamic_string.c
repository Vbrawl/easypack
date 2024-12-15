#include <string.h>
#include <stdio.h>
#include "dynamic_string.h"
#include "testutils.h"

#define TEST_INCREMENTOR 3

#define TEST_ADDSTRING_VAL1 "Hello "
#define TEST_ADDSTRING_VAL2 "World!"

int test_addString() {
  struct dstring str = {0};

  int err = dstring_addString(&str, TEST_ADDSTRING_VAL1, strlen(TEST_ADDSTRING_VAL1));
  if(err != 0) {
    dstring_clearAll(&str);
    return 1;
  }

  err = dstring_addString(&str, TEST_ADDSTRING_VAL2, strlen(TEST_ADDSTRING_VAL2));
  if(err != 0) {
    dstring_clearAll(&str);
    return 2;
  }

  if(strcmp(str.buf, TEST_ADDSTRING_VAL1 TEST_ADDSTRING_VAL2) != 0) {
    dstring_clearAll(&str);
    return 3;
  }

  dstring_clearAll(&str);
  return 0;
}

int main() {
  int status = 0;
  int counter = 0;

  RUN_TEST(status, counter, TEST_INCREMENTOR, test_addString);
  return status;
}
