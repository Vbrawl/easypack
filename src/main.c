#include <stdio.h>
#include "file_sections.h"
#include "embedded_filesystem.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <limits.h>
#include "utils.h"
#include "string_array.h"


int createPackage(const char *root, const char *out) {
  char *exe_name = NULL, *system_data = NULL;
  uint32_t exe_size = 0, system_size = 0;
  struct fs *system = NULL;

  // Get executable's name ...
  if((exe_name = getExecutableName()) == NULL)
    return 1;

  // ... and size
  if((exe_size = getExecutableSize()) == 0) {
    free(exe_name);
    return 2;
  }

  // Add everything to memory
  if((system = loadFileSystem(root)) == NULL) {
    free(exe_name);
    return 3;
  }

  // Extract required info from the system
  system_size = calculateFileSystemAsDataLength(system);
  if((system_data = exportFileSystemAsData(system, system_size)) == NULL) {
    free(system_data);
    unLoadFileSystem(system);
    free(exe_name);
    return 4;
  }

  // Export the package
  setEmbeddedData(exe_name, out, system_data, system_size);

  // Clean everything
  free(system_data);
  unLoadFileSystem(system);
  free(exe_name);
  return 0;
}

int main() {
  uint32_t data_size = 0;
  char *data = NULL;
  const char *root = "awd";
  const char *out_name = "easypack.new";
  const char *out_dir = "dumped";
  struct fs *system = NULL;

  uint32_t data_size = getEmbeddedDataSize(NULL);
  printf("Data Size: %u\n", data_size);

  if(data_size == 0) {
    createPackage(root, out_name);
  }
  else {
    data = getEmbeddedData(NULL);
    system = loadFileSystemFromData(data);
    dumpFileSystem(system, out_dir);

    unLoadFileSystem(system);
    free(data);
  }

  return 0;
}
