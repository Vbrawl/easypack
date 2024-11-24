#ifndef EASYPACK_EMBEDDED_FILESYSTEM_H
#define EASYPACK_EMBEDDED_FILESYSTEM_H

#include <stdint.h>

struct fs_item {
  char *filename;
  char *data;
  uint32_t fsize;
  uint32_t dsize;
};

struct fs {
  struct fs_item *files;
  uint32_t size;
};

struct fs* loadFileSystemFromData(char* data);
uint32_t calculateFileSystemAsDataLength(struct fs *system);
char* exportFileSystemAsData(struct fs *system, uint32_t size);
void unLoadFileSystem(struct fs *system);
#endif
