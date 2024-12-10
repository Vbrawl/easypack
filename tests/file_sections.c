#include <string.h>
#include <stdio.h>
#include "file_sections.h"
#include "utils.h"
#include "testutils.h"
#include <stdlib.h>

#define TEST_INCREMENTOR 3

#define TEST_EXECUTABLE_NAME "test_file_sections"

int test_getExecutableName() {
  char *exe_name = getExecutableName();
  char *cwd = getCurrentWorkingDirectory();
  const char *name = TEST_EXECUTABLE_NAME;
  char *res = pathJoin(cwd, name);
  free(cwd);

  if(strcmp(exe_name, res) != 0) {
    free(exe_name);
    free(res);
    return 1;
  }

  free(exe_name);
  free(res);
  return 0;
}

int main() {
  int status = 0;
  int counter = 0;

  RUN_TEST(status, counter, TEST_INCREMENTOR, test_getExecutableName);
  return status;
}
