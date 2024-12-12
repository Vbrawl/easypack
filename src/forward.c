#include "forward.h"
#include "file_sections.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>


API int prepareEnvironment() {
  char *cwd = NULL, *name = NULL;

  if((cwd = getCurrentWorkingDirectory()) == NULL) {
    return -1;
  }

  if((name = getExecutableName()) == NULL) {
    free(cwd);
    return -2;
  }

  if(setEnvironmentVariable(ENVOUT_EASYPACK_CWD, cwd) != 0) {
    free(cwd);
    free(name);
    return -3;
  }

  if(setEnvironmentVariable(ENVOUT_EASYPACK_NAME, name) != 0) {
    free(cwd);
    free(name);
    return -4;
  }

  free(cwd);
  free(name);
  return 0;
}


API void executeAutoRun(const char *autorun_name, char *const *argv) {
  markAsExecutable(autorun_name);
  execAndReplace(autorun_name, argv);
}

