#include "addons.h"
#include "string_array.h"
#include <stdio.h>
#include <stdlib.h>

int executeAddons(struct fs *system) {
  const char *addon_list = NULL;
  char *cursor = NULL;
  struct sarray addons = {0};
  int status = 0;

  addon_list = getenv(ENV_EASYPACK_ADDONS);
  if(addon_list == NULL) return 0;

  status = sarray_addStringsFromList(&addons, addon_list, EASYPACK_ADDONS_SEPARATOR);
  if(status != 0) return -1;

  while((cursor = sarray_getNextString(&addons, cursor)) != NULL) {
    executeSingleAddon(cursor, system);
  }

  return 0;
}
