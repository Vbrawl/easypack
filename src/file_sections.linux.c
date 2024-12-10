#include "file_sections.h"

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

char* getExecutableName() {
  char *name = NULL;
  ssize_t nsize = 0;

  if((name = malloc(PATH_MAX + 1)) == NULL) {
    perror("getExecutableName()");
    return NULL;
  }

  if((nsize = readlink("/proc/self/exe", name, PATH_MAX)) == -1) {
    free(name);
    perror("getExecutableName()");
    return NULL;
  }
  name[nsize] = '\0';

  return name;
}
