#include <stdio.h>
#include "file_sections.h"
#include "embedded_filesystem.h"
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"
#include "forward.h"
#include "addons.h"
#include <limits.h>
#include <string.h>

#define UNUSED(x) (void)(x)

#define ENV_EASYPACK_ROOT         "EASYPACK_ROOT"
#define ENV_EASYPACK_OUT          "EASYPACK_OUT"
#define ENV_EASYPACK_MOUNTPOINTS  "EASYPACK_MOUNTPOINTS"
#define MOUNTPOINT_SEPARATOR      ','
#define MOUNTPOINT_DIRECTION_SEPARATOR ':'

int createPackage(const char *root, const char *out, struct sarray *mountpoints) {
  char *exe_name = NULL, *system_data = NULL, *cursor = NULL;
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

  // Add all mountpoints to virtual filesystem
  while((cursor = sarray_getNextString(mountpoints, cursor)) != NULL) {
    char *src = NULL, *dst = NULL;

    if(splitOnce(cursor, strlen(cursor), &src, &dst, MOUNTPOINT_DIRECTION_SEPARATOR) == -1) {
      printf("An error occured while adding mountpoint!\n");
      unLoadFileSystem(system);
      free(exe_name);
      return -1;
    }

    struct fs *other = loadFileSystem(src);

    if(extendFileSystem(system, other, dst) == -1) {
      unLoadFileSystem(system);
      unLoadFileSystem(other);
      free(exe_name);
      return -1;
    }

    unLoadFileSystem(other);

    free(src);
    free(dst);
  }

  // Call all addons
  if(executeAddons(system) != 0) {
    printf("An error occured while executing addons!\n");
    unLoadFileSystem(system);
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

  chmod(out, S_IWUSR | S_IRUSR | S_IXUSR | S_IXGRP | S_IXOTH);

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

  unLoadFileSystem(system);
  free(data);
  return 0;
}

/**
 * @retval 0  Success
 * @retval -1 Failed
 * @retval -2 Failed is changed
 */
int extractPackageAndAutoRun(const char *out_dir, char *const *argv) {
  char prev_path[PATH_MAX];
  int error = 0;

  if(getcwd(prev_path, PATH_MAX) == NULL) return -1;
  if(prepareEnvironment() != 0) return -1;

  error = extractPackage(out_dir);
  if(error != 0) return -1;

  error = chdir(out_dir);
  if(error != 0) return -1;

  if(access(EASYPACK_AUTORUN_NAME, F_OK) == 0) {
    executeAutoRun(EASYPACK_AUTORUN_NAME, argv);
    error = chdir(prev_path);
    if(error != 0) return -2;
    return -1;
  }
  return 0;
}

int main(int argc, char *const *argv) {
  UNUSED(argc);

  const char *root = getenv(ENV_EASYPACK_ROOT);
  const char *out = getenv(ENV_EASYPACK_OUT);

  struct sarray mountpoints = {0};
  const char *mountpoints_str = getenv(ENV_EASYPACK_MOUNTPOINTS);

  const char *mkdtemp_template = "/tmp/easypack_";


  uint32_t dsize = 0;
  dsize = getEmbeddedDataSize(NULL);
  if(dsize > 0) {
    if(out == NULL) {
      out = make_temp_directory(mkdtemp_template);
      if(out == NULL) return -1;
    }
    extractPackageAndAutoRun(out, argv);
  }
  else {
    if(root == NULL) {
      printf("Package empty!\nCreate a new package by setting the required environment variables!\n");
      return -1;
    }

    if(out == NULL) {
      printf("Can't output new package without a proper filename!\n");
      return -1;
    }

    if(mountpoints_str != NULL) {
      if(sarray_addStringsFromList(&mountpoints, mountpoints_str, MOUNTPOINT_SEPARATOR) == -1) {
        printf("Couldn't parse mountpoints!\n");
        return -1;
      }
    }

    createPackage(root, out, &mountpoints);
  }

  return 0;
}
