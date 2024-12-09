#include <string.h>
#include <stdio.h>
#include "utils.h"
#include "testutils.h"
#include <stdlib.h>

#define TEST_INCREMENTOR 3

#define TEST_SPLITONCE_PART1 "Hello"
#define TEST_SPLITONCE_PART2 "World"

int test_splitOnce() {
  const char *data = TEST_SPLITONCE_PART1 " " TEST_SPLITONCE_PART2;
  char *part1 = NULL, *part2 = NULL;

  int err = splitOnce(data, strlen(data), &part1, &part2, ' ');
  if(err != 0) return 1;
  if(strcmp(part1, TEST_SPLITONCE_PART1) != 0) {
    free(part1);
    free(part2);
    return 2;
  }
  if(strcmp(part2, TEST_SPLITONCE_PART2) != 0) {
    free(part1);
    free(part2);
    return 3;
  }

  free(part1);
  free(part2);
  return 0;
}


int main() {
  int status = 0;
  int counter = 0;

  RUN_TEST(status, counter, TEST_INCREMENTOR, test_splitOnce);
  return status;
}
