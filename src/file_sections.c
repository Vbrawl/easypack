#include "file_sections.h"
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>


char* getExecutableName() {
  // Try to allocate enough space
  size_t BUF_SIZE = 0;
  ssize_t lnksize = 0;
  char *name = NULL,
  *name_bak = NULL;

  do {
    // Update buffer size
    BUF_SIZE += sizeof(char) * MAX_FILENAME_SIZE;

    // Reallocate buffer
    name = realloc(name, BUF_SIZE);
    if(name == NULL) {
      // in failure clear everything
      perror("getExecutableName()");
      free(name_bak);
      return NULL;
    }
    // Update name_bak which is used only for clearing
    // the buffer in case of failure
    name_bak = name;

    // Read link name
    lnksize = readlink("/proc/self/exe", name, BUF_SIZE);
    if(lnksize == -1) {
      perror("getExecutableName()");
      free(name);
      return NULL;
    }

    // readlink doesn't add a NULL-byte
    name[lnksize] = '\0';
  } while(lnksize == BUF_SIZE);

  // finally return the executable name
  return name;
}

uint32_t getExecutableSize() {
  struct stat info;
  int err;
  FILE* f = NULL;
  off_t fsize = 0;
  uint32_t embedded_size = 0;
  size_t rbytes;
  char* exe_name = getExecutableName();
  if(exe_name == NULL) {
    return 0;
  }

  // Get file size
  err = stat(exe_name, &info);
  if(err == -1) {
    perror("getExecutableSize()");
    free(exe_name);
    return 0;
  }
  fsize = info.st_size;

  // Open exe
  f = fopen(exe_name, "rb");
  if(f == NULL) {
    perror("getExecutableSize()");
    free(exe_name);
    return 0;
  }

  // Read the last 4 bytes
  fseek(f, fsize-sizeof(uint32_t), SEEK_SET);
  rbytes = fread(&embedded_size, sizeof(uint32_t), 1, f);
  if(rbytes != 1) {
    printf("getExecutableSize(): Couln't read embedded size, only %ld bytes read!\n", rbytes);
    free(exe_name);
    return 0;
  }

  // clean up
  err = fclose(f);
  free(exe_name);
  return fsize - embedded_size - sizeof(uint32_t);
}
