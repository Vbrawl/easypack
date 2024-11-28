#include <stdio.h>
#include "file_sections.h"
#include "embedded_filesystem.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "utils.h"
#include "string_array.h"


int main() {
  char* exe_name = getExecutableName();
  if(exe_name == NULL) {
    printf("Couldn't resolve executable name!\n");
    return 1;
  }
  printf("Name: %s\n", exe_name);

  uint32_t exe_size = getExecutableSize();
  printf("Executable Size: %u\n", exe_size);

  uint32_t data_size = getEmbeddedDataSize(NULL);
  printf("Data Size: %u\n", data_size);

  char* data = getEmbeddedData(NULL);
  if(data == NULL) {
    printf("Data: NULL\n");
    printf("Adding fake data!\n");

    const char *root = "awd";
    struct fs *system = loadFileSystem(root);
    //if(system == NULL) {
    // TODO: Handle error
    //}
    uint32_t system_size = calculateFileSystemAsDataLength(system);
    char *system_data = exportFileSystemAsData(system, system_size);

    // process new filename
    size_t namesize = strlen(exe_name);
    char* new_name = malloc(namesize + 5);
    memcpy(new_name, exe_name, strlen(exe_name));
    new_name[namesize] = '.';
    new_name[namesize + 1] = 'n';
    new_name[namesize + 2] = 'e';
    new_name[namesize + 3] = 'w';
    new_name[namesize + 4] = '\0';

    // set embedded data
    setEmbeddedData(exe_name, new_name, system_data, system_size);

    free(new_name);
    unLoadFileSystem(system);
    free(exe_name);
    return 0;
  }
  else {
    printf("Data: %s\n", data);
  }

  struct fs *system = loadFileSystemFromData(data);

  dumpFileSystem(system, "dumped");

  unLoadFileSystem(system);
  free(data);
  free(exe_name);
  return 0;
}
