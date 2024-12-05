#include "forward.h"
#include "file_sections.h"
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int prepareEnvironment() {
  char cwd[PATH_MAX], *name;

  if(getcwd(cwd, PATH_MAX) == NULL) {
    perror("getcwd");
    return -1;
  }

  if((name = getExecutableName()) == NULL) {
    return -2;
  }

  if(setenv(ENVOUT_EASYPACK_CWD, cwd, 1) != 0) {
    perror("setenv");
    return -3;
  }

  if(setenv(ENVOUT_EASYPACK_NAME, name, 1) != 0) {
    perror("setenv");
    return -4;
  }

  free(name);
  return 0;
}


void executeAutoRun(const char *autorun_name, char *const *argv) {
  chmod(autorun_name, S_IWUSR | S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  execv(autorun_name, argv);
  perror("executeAutoRun -> execv");
}

