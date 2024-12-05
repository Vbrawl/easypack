#include <stdio.h>
#include "file_sections.h"
#include "embedded_filesystem.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <getopt.h>
#include "utils.h"
#include "string_array.h"
#include "forward.h"


#define ENV_EASYPACK_ROOT "EASYPACK_ROOT"
#define ENV_EASYPACK_OUT  "EASYPACK_OUT"

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
  printf("Package created!\n");

  // Clean everything
  free(system_data);
  unLoadFileSystem(system);
  free(exe_name);
  return 0;
}

int extractPackage(const char *out_dir) {
  uint32_t dsize = 0;
  char *data = NULL;
  struct fs *system = NULL;

  dsize = getEmbeddedDataSize(NULL);
  if(dsize == 0) {
    printf("Nothing to extract.\n");
    return -1;
  }
  data = getEmbeddedData(NULL);
  system = loadFileSystemFromData(data);
  dumpFileSystem(system, out_dir);

  printf("Package extracted!\n");

  unLoadFileSystem(system);
  free(data);
  return 0;
}

int extractPackageAndAutoRun(const char *out_dir, char *const *argv) {
  if(prepareEnvironment() != 0) return -1;

  extractPackage(out_dir);
  chdir(out_dir);
  if(access(EASYPACK_AUTORUN_NAME, F_OK) == 0) {
    executeAutoRun(EASYPACK_AUTORUN_NAME, argv);
    return -2;
  }
  return 0;
}

/**
 * Make sure to free the return value if it's not NULL.
 */
char* make_temp_directory(const char *template) {
  size_t template_len = strlen(template);
  size_t X_append = 6;

  char *malloc_template = malloc(template_len + X_append + 1);
  if(malloc_template == NULL) return NULL;
  strncpy(malloc_template, template, template_len);
  memset(malloc_template + template_len, 'X', X_append);
  malloc_template[template_len + X_append] = '\0';

  if(mkdtemp(malloc_template) == NULL) {
    perror("mkdtemp");
    free(malloc_template);
    return NULL;
  }

  return malloc_template;
}

int main(int argc, char *const *argv) {
  int opt = 0, option_index = 0;

  const char *root = getenv(ENV_EASYPACK_ROOT);
  const char *out = getenv(ENV_EASYPACK_OUT);
  const char *mkdtemp_template = "/tmp/easypack_";

  if(root != NULL) {
    if(out == NULL) {
      printf("Can't output new package without a proper filename\n");
      return -1;
    }
    createPackage(root, out);
  }
  else {
    if(out == NULL) {
      out = make_temp_directory(mkdtemp_template);
      if(out == NULL) return -1;
    }
    extractPackageAndAutoRun(out, argv);
  }

  return 0;
}
