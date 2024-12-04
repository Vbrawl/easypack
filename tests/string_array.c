#include <string.h>
#include <stdio.h>
#include "string_array.h"



int test_getNextString() {
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

  return 0;
}


int main() {
    return test_getNextString();
}
