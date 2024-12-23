#include "addons.h"
#include <windows.h>
#include <stdio.h>

API int executeSingleAddon(const char *name, struct fs *system) {
  HMODULE handle;
  DWORD error_message = 0;
  union AddonSymbol sym_execute;
  int status = 0;

  handle = LoadLibrary(name);
  if(handle == NULL) {
    error_message = GetLastError();
    if(error_message) printf("executeSingleAddon(): %lu\n", error_message);
    return -1;
  }

  sym_execute.raw = GetProcAddress(handle, "execute");
  if(sym_execute.raw == NULL) {
    error_message = GetLastError();
    if(error_message) printf("executeSingleAddon(): %lu\n", error_message);
    FreeLibrary(handle);
    return -1;
  }

  status = sym_execute.Fexecute(system);
  FreeLibrary(handle);

  if(status != 0) return -1;

  return 0;
}
