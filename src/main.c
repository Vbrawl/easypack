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
  // Get a copy of CWD
  char cwd[PATH_MAX];
  if(getcwd(cwd, PATH_MAX) == NULL) {
    perror("createPackage()");
    return 1;
  }

  // Go to "root"
  //if(chdir(root) != 0) {
  //  perror("createPackage()");
  //  return 2;
  //}

  // Get executable's name ...
  char *exe_name = getExecutableName();
  if(exe_name == NULL) {
    return 3;
  }

  // .. and size
  uint32_t exe_size = getExecutableSize();
  if(exe_size == 0) {
    free(exe_name);
    return 4;
  }

  // Add everything to memory
  struct fs *system = loadFileSystem(root);
  if(system == NULL) {
    free(exe_name);
    return 5;
  }

  // Extract required info from the system
  uint32_t system_size = calculateFileSystemAsDataLength(system);
  char *system_data = exportFileSystemAsData(system, system_size);
  if(system_data == NULL) {
    free(system_data);
    free(exe_name);
    return 6;
  }

  // Go back to real CWD
  //if(chdir(cwd) != 0) {
  //  perror("createPackage()");
  //  free(exe_name);
  //  return 7;
  //}

  // Export the package
  setEmbeddedData(exe_name, out, system_data, system_size);

  // Clean everything
  free(system_data);
  unLoadFileSystem(system);
  free(exe_name);
  return 0;
}

int main() {
  uint32_t data_size = getEmbeddedDataSize(NULL);
  printf("Data Size: %u\n", data_size);

  if(data_size == 0) {
    const char *root = "awd";
    const char *out_name = "easypack.new";
    createPackage(root, out_name);
  }
  else {
    char *data = getEmbeddedData(NULL);
    struct fs *system = loadFileSystemFromData(data);

    const char *out_dir = "dumped";
    dumpFileSystem(system, out_dir);

    unLoadFileSystem(system);
    free(data);
  }

  return 0;
}
