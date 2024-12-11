#include "utils.h"

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

size_t getFileSize(const char *name) {
  struct stat s = {0};

  if(stat(name, &s) != 0) {
    perror("getFileSize()");
    return (size_t)-1;
  }

  return (size_t)s.st_size;
}

char* getCurrentWorkingDirectory() {
  return getcwd(NULL, 0); // Dynamically allocated buffer
}

int makeDirectory(const char *path) {
  int err = mkdir(path, 0777);
  if(err == -1 && errno != EEXIST) return -1;
  return 0;
}

int listDirectory(const char *dirpath, struct sarray *arr, unsigned char type) {
  DIR *d = NULL;
  struct dirent *dentry = NULL;
  size_t fname_size = 0;

  sarray_clearAll(arr);

  // Open directory
  d = opendir(dirpath);
  if(d == NULL) {
    perror("listDirectory()");
    return -1;
  }

  // Read all entries in the directory
  while((dentry = readdir(d)) != NULL) {
    if(dentry->d_type != type) continue;
    if(dentry->d_name[0] == '.' && dentry->d_name[1] == '\0') continue;
    if(dentry->d_name[0] == '.' && dentry->d_name[1] == '.' && dentry->d_name[2] == '\0') continue;

    // Get name size
    fname_size = strlen(dentry->d_name);

    // add name to sarray
    if(sarray_addString(arr, dentry->d_name, fname_size) != 0) {
      perror("listDirectory()");
      sarray_clearAll(arr);
      closedir(d);
      return -1;
    }
  }

  // Cleanup
  closedir(d);
  return 0;
}

char* make_temp_directory(const char* template) {
  size_t template_len = strlen(template);
  size_t X_append = 6;

  char* malloc_template = malloc(template_len + X_append + 1);
  if (malloc_template == NULL) return NULL;
  memcpy(malloc_template, template, template_len);
  memset(malloc_template + template_len, 'X', X_append);
  malloc_template[template_len + X_append] = '\0';

  if (mkdtemp(malloc_template) == NULL) {
    perror("mkdtemp");
    free(malloc_template);
    return NULL;
  }

  return malloc_template;
}
