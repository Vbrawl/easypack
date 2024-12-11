#include "file_sections.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

char* getExecutableName() {
  char *name = NULL;
  size_t nsize = 0;

  if((name = malloc(MAX_PATH + 1)) == NULL) {
    perror("getExecutableName()");
    return NULL;
  }

  if((nsize = GetModuleFileName(NULL, name, MAX_PATH + 1)) == 0) {
    printf("getExecutableName(): %lu\n", GetLastError());
    free(name);
    return NULL;
  }
  name[nsize] = '\0';

  return name;
}
