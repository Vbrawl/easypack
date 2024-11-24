#include "embedded_filesystem.h"
#include <string.h>
#include <stdlib.h>


struct fs* loadFileSystemFromData(char* data) {
  struct fs *system = malloc(sizeof(struct fs));

  memcpy(&system->size, data, sizeof(system->size));
  system->files = malloc(system->size * sizeof(struct fs_item));
  data += sizeof(system->size);

  for(size_t i = 0; i < system->size; i++) {
    struct fs_item *item = &system->files[i];

    // Read filename
    memcpy(&item->fsize, data, sizeof(item->fsize));
    data += sizeof(item->fsize);
    item->filename = malloc(sizeof(char) * item->fsize);
    memcpy(item->filename, data, sizeof(char) * item->fsize);
    data += item->fsize;

    // Read data
    memcpy(&item->dsize, data, sizeof(item->dsize));
    data += sizeof(item->dsize);
    item->data = malloc(sizeof(char) * item->dsize);
    memcpy(item->data, data, sizeof(char) * item->dsize);
    data += item->dsize;
  }

  return system;
}

void unLoadFileSystem(struct fs *system) {
  for(uint32_t i = 0; i < system->size; i++) {
    struct fs_item *item = &system->files[i];
    free(item->filename);
    free(item->data);
  }

  free(system->files);
  free(system);
}

uint32_t calculateFileSystemAsDataLength(struct fs *system) {
  uint32_t size = 0;

  // filesystem items number length
  size += sizeof(system->size);

  for(uint32_t i = 0; i < system->size; i++) {
    struct fs_item *item = &system->files[i];
    size += sizeof(item->fsize);
    size += item->fsize;
    size += sizeof(item->dsize);
    size += item->dsize;
  }

  return size;
}

char* exportFileSystemAsData(struct fs *system, uint32_t size) {
  char* buf = malloc(sizeof(char) * size);
  char* buf_backup = buf;

  memcpy(buf, &system->size, sizeof(system->size));
  buf += sizeof(system->size);

  for(uint32_t i = 0; i < system->size; i++) {
    struct fs_item *item = &system->files[i];

    memcpy(buf, &item->fsize, sizeof(item->fsize));
    buf += sizeof(item->fsize);
    memcpy(buf, item->filename, item->fsize);
    buf += item->fsize;

    memcpy(buf, &item->dsize, sizeof(item->dsize));
    buf += sizeof(item->dsize);
    memcpy(buf, item->data, item->dsize);
    buf += item->dsize;
  }

  return buf_backup;
}
