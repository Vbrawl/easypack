#include "addons.h"
#include <dlfcn.h>
#include <stdio.h>

int executeSingleAddon(const char *name, struct fs *system) {
  void *handle = NULL;
  const char *error_message = NULL;
  union AddonSymbol sym_execute;
  int status = 0;

  handle = dlopen(name, RTLD_LAZY);
  if(handle == NULL) {
    error_message = dlerror();
    if(error_message) printf("executeSingleAddon(): %s\n", error_message);
    return -1;
  }

  sym_execute.raw = dlsym(handle, "execute");
  if(sym_execute.raw == NULL) {
    error_message = dlerror();
    if(error_message) printf("executeSingleAddon(): %s\n", error_message);
    dlclose(handle);
    return -1;
  }

  status = sym_execute.Fexecute(system);
  dlclose(handle);

  if(status != 0) return -1;

  return 0;
}
