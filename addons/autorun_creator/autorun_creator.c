#include <stdio.h>
#include "easypack_addon.h"
#include "string_array.h"
#include <string.h>
#include <stdlib.h>

#define AUTORUN_FILENAME "autorun.easypack"
#define AUTORUN_DATA \
"#!/bin/sh\n"\
"F=\"%s\"\n"\
"chmod +x \"$F\" %s\n"\
"./$F"

#define ENV_AUTORUNCREATOR_FILENAME "ARC_FILE"
#define ENV_AUTORUNCREATOR_BINARIES "ARC_EXECUTABLES"

char* quotedJoin(struct sarray *list) {
  char *data = NULL, *tmp = NULL, *cursor = NULL;
  size_t csize = 0;
  size_t dsize = list->buf_size + (list->count * 2);

  if(dsize == 0) {
    data = malloc(1);
    *data = '\0';
    return data;
  }

  tmp = data = malloc(dsize + 1);
  while((cursor = sarray_getNextString(list, cursor)) != NULL) {
    if(tmp != data) {
      *tmp = ' ';
      tmp++;
    }
    csize = strlen(cursor);

    *tmp = '"';
    tmp++;
    memcpy(tmp, cursor, csize);
    tmp += csize;
    *tmp = '"';
    tmp++;
  }
  data[dsize] = '\0';
  return data;
}

int execute(struct fs *system) {
  const char *executable = NULL, *executables_list = NULL;
  struct sarray executables = {0};
  char *data = NULL, *executables_quoted_list = NULL;
  size_t dsize = 0;

  executable = getenv(ENV_AUTORUNCREATOR_FILENAME);
  if(executable == NULL) {
    printf("Define \"" ENV_AUTORUNCREATOR_FILENAME  "\" environment variable to proceed!\n");
    return -1;
  }

  executables_list = getenv(ENV_AUTORUNCREATOR_BINARIES);
  if(executables_list != 0) {
    if(sarray_addStringsFromList(&executables, executables_list, ',') != 0) {
      return -1;
    }
  }
  executables_quoted_list = quotedJoin(&executables);

  dsize = snprintf(NULL, 0, AUTORUN_DATA, executable, executables_quoted_list);
  data = malloc(dsize + 1);
  if(data == NULL) {
    printf("Not enough memory!");
    free(executables_quoted_list);
    sarray_clearAll(&executables);
    return -1;
  }
  snprintf(data, dsize, AUTORUN_DATA, executable, executables_quoted_list);
  data[dsize] = '\0';

  addFileToFileSystem(system, AUTORUN_FILENAME, data, dsize);
  free(data);
  free(executables_quoted_list);
  sarray_clearAll(&executables);
  return 0;
}
