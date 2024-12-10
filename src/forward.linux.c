#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int setEnvironmentVariable(const char *name, const char *value) {
  if(setenv(name, value, 1) != 0) {
    perror("setEnvironment()");
    return -1;
  }
  return 0;
}

int markAsExecutable(const char *name) {
  struct stat s = {0};
  mode_t modifiers = S_IXUSR | S_IRUSR
                    | S_IXGRP | S_IRGRP
                    | S_IXOTH | S_IROTH;

  if(stat(name, &s) != 0) {
    perror("markAsExecutable()");
    return -1;
  }

  if(chmod(name, s.st_mode | modifiers) != 0) {
    perror("markAsExecutable()");
    return -1;
  }
  return 0;
}


void execAndReplace(const char *name, char *const *argv) {
  execv(name, argv);
  perror("execAndReplace()");
}
