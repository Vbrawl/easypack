#include <stdio.h>
#include <easypack_addon.h>
#include <string.h>
#include <stdlib.h>

#define AUTORUN_FILENAME "autorun.easypack"
#define AUTORUN_DATA \
"#!/bin/sh\n"\
"F=\"%s\"\n"\
"chmod +x \"$F\"\n"\
"./$F"

#define ENV_AUTORUNCREATOR_FILENAME "ARC_FILE"


int execute(struct fs *system) {
  const char *executable = getenv(ENV_AUTORUNCREATOR_FILENAME);
  if(executable == NULL) {
    printf("Define \"%s\" environment variable to proceed!\n");
    return -1;
  }

  size_t dsize = snprintf(NULL, 0, AUTORUN_DATA, executable);
  char *data = malloc(dsize + 1);
  if(data == NULL) {
    printf("Not enough memory!");
    return -1;
  }
  snprintf(data, dsize + 1, AUTORUN_DATA, executable);

  addFileToFileSystem(system, AUTORUN_FILENAME, data, dsize);
  free(data);
  return 0;
}
