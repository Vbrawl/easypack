#include <string.h>
#include <stdio.h>
#include "string_array.h"
#include "testutils.h"

#define TEST_INCREMENTOR 3

char test_getString() {
  struct sarray arr = {0};
  const char *hi = "hi";
  const char *hello = "hello";

  sarray_addString(&arr, hi, strlen(hi));
  sarray_addString(&arr, hello, strlen(hello));

  char *cursor = sarray_getString(&arr, 0);
  if(strcmp(cursor, hi) != 0) {
    sarray_clearAll(&arr);
    return 1;
  }

  cursor = sarray_getString(&arr, 1);
  if(strcmp(cursor, hello) != 0) {
    sarray_clearAll(&arr);
    return 2;
  }

  cursor = sarray_getString(&arr, 2);
  if(cursor != NULL) {
    sarray_clearAll(&arr);
    return 3;
  }

  sarray_clearAll(&arr);
  return 0;
}

char test_getNextString() {
  struct sarray arr = {0};
  const char *hi = "hi";
  const char *hello = "hello";

  sarray_addString(&arr, hi, strlen(hi));
  sarray_addString(&arr, hello, strlen(hello));

  char *cursor = sarray_getNextString(&arr, NULL);
  if(strcmp(cursor, hi) != 0) {
    sarray_clearAll(&arr);
    return 1;
  }

  cursor = sarray_getNextString(&arr, cursor);
  if(strcmp(cursor, hello) != 0) {
    sarray_clearAll(&arr);
    return 2;
  }

  cursor = sarray_getNextString(&arr, cursor);
  if(cursor != NULL) {
    sarray_clearAll(&arr);
    return 3;
  }

  sarray_clearAll(&arr);
  return 0;
}

int test_addStringsFromList() {
  struct sarray arr = {0};

  const char *item1 = "hi";
  const char *item2 = "hello";
  const char *item3 = "awd";
  sarray_addStringsFromList(&arr, "hi,hello,awd", ',');

  char *cursor = sarray_getString(&arr, 0);
  if(strcmp(cursor, item1) != 0) {
    sarray_clearAll(&arr);
    return 1;
  }

  cursor = sarray_getString(&arr, 1);
  if(strcmp(cursor, item2) != 0) {
    sarray_clearAll(&arr);
    return 2;
  }

  cursor = sarray_getString(&arr, 2);
  if(strcmp(cursor, item3) != 0) {
    sarray_clearAll(&arr);
    return 3;
  }

  cursor = sarray_getString(&arr, 3);
  if(cursor != NULL) {
    sarray_clearAll(&arr);
    return 4;
  }

  sarray_clearAll(&arr);
  return 0;
}

int main() {
  int status = 0;
  int counter = 0;

  RUN_TEST(status, counter, TEST_INCREMENTOR, test_getString);
  RUN_TEST(status, counter, TEST_INCREMENTOR, test_getNextString);
  RUN_TEST(status, counter, TEST_INCREMENTOR, test_addStringsFromList);
  return status;

//  if((status = test_getString()) & (~mask) != 0) return status;

//  return test_getNextString();
}
