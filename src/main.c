#include <stdio.h>
#include "file_sections.h"
#include "embedded_filesystem.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include "utils.h"
#include "string_array.h"


int createPackage(const char *root, const char *out) {
  char *exe_name = NULL, *system_data = NULL;
  uint32_t system_size = 0;
  struct fs *system = NULL;

  // Get executable's name ...
  if((exe_name = getExecutableName()) == NULL)
    return 1;

  // Add everything to memory
  if((system = loadFileSystem(root)) == NULL) {
    free(exe_name);
    return 2;
  }

  // Extract required info from the system
  system_size = calculateFileSystemAsDataLength(system);
  if((system_data = exportFileSystemAsData(system, system_size)) == NULL) {
    free(system_data);
    unLoadFileSystem(system);
    free(exe_name);
    return 3;
  }

  // Export the package
  setEmbeddedData(exe_name, out, system_data, system_size);

  // Clean everything
  free(system_data);
  unLoadFileSystem(system);
  free(exe_name);
  return 0;
}

int extractPackage(const char *out_dir) {
  char *data = NULL;
  struct fs *system = NULL;

  data = getEmbeddedData(NULL);
  system = loadFileSystemFromData(data);
  dumpFileSystem(system, out_dir);

  unLoadFileSystem(system);
  free(data);
  return 0;
}

int main(int argc, char *const *argv) {
  uint32_t data_size = 0;
  const char *root = "awd";
  const char *out_name = "easypack.new";
  const char *out_dir = "dumped";
  const char *autorun_name = "autorun.easypack";
  int err = 0;

  data_size = getEmbeddedDataSize(NULL);
  printf("Data Size: %u\n", data_size);

  if(data_size == 0) {
    createPackage(root, out_name);
  }
  else {
    extractPackage(out_dir);
    chdir(out_dir);
    if(access(autorun_name, F_OK) == 0) {
      chmod(autorun_name, S_IWUSR | S_IRUSR | S_IXUSR | S_IWGRP | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
      execv(autorun_name, argv);
      perror("main -> execv");
    }
  }

  return 0;
}
