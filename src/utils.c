#include "utils.h"
#include <stdio.h>
#include <string.h>


int makedirs(char* path, size_t pathsize, mode_t mode) {
  if(path[0] == '/') return 0;
  if(path[0] == '.' && path[1] == '\0') return 0;

  // Replace all "/" with NULL-byte, only skip the first
  size_t components = 1;
  for(size_t i = 1; i < pathsize; i++) {
    if(path[i-1] == '\\') continue;
    if(path[i] == '/') {
      path[i] = '\0';
      components++;
    }
  }

  // Create component and recover slash
  for(size_t i = 0; i < components; i++) {
    if(mkdir(path, mode) == -1) {
      perror("makedirs()");
      return -1;
    }

    size_t pathlen = strlen(path);
    path[pathlen] = '/';
  }

  return 0;
}
