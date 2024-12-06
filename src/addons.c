#include "addons.h"
#include "string_array.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int executeAddons(struct fs *system) {
  const char *addon_list = NULL;
  char *cursor = NULL;
  char *filename = NULL;
  void *handle = NULL;
  struct sarray addons = {0};
  size_t i = 0;
  int status = 0;
  const char *error_message = NULL;
  int(*execute_addon)(struct fs*) = NULL;

  addon_list = getenv(ENV_EASYPACK_ADDONS);
  if(addon_list == NULL) return 0;

  status = sarray_addStringsFromList(&addons, addon_list, EASYPACK_ADDONS_SEPARATOR);
  if(status != 0) return -1;

  while((cursor = sarray_getNextString(&addons, cursor)) != NULL) {
    handle = dlopen(cursor, RTLD_LAZY);
    if(handle == NULL) {
      error_message = dlerror();
      if(error_message) printf("execute_addons -> dlopen: %s\n", error_message);
      sarray_clearAll(&addons);
      return -1;
    }

    execute_addon = dlsym(handle, "execute");
    if(execute_addon == NULL) {
      error_message = dlerror();
      if(error_message) printf("execute_addons -> dlsym: %s\n", error_message);
      dlclose(handle);
      sarray_clearAll(&addons);
      return -1;
    }

    status = execute_addon(system);
    dlclose(handle);

    if(status != 0) return -1;
  }

  return 0;
}
