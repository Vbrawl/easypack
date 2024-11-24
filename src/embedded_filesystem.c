#include "embedded_filesystem.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <libgen.h>


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
    item->filename = malloc(sizeof(char) * item->fsize + 1);
    memcpy(item->filename, data, sizeof(char) * item->fsize);
    item->filename[item->fsize] = '\0';
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

void dumpFileSystem(struct fs *system, char* dir_name) {
  int err = 0;
  size_t dir_name_size = strlen(dir_name);
  for(size_t i = 0; i < system->size; i++) {
    struct fs_item *item = &system->files[i];

    // Combine filename with dir_name
    char *final_filename = malloc(sizeof(char) * (dir_name_size + 1 + item->fsize + 1));
    memcpy(final_filename, dir_name, dir_name_size);
    final_filename[dir_name_size] = '/';
    memcpy(final_filename + 1 + dir_name_size, item->filename, item->fsize);
    final_filename[dir_name_size + 1 + item->fsize + 1] = '\0';

    // Create copy
    char *filename_copy_d = strdup(final_filename);

    // this may edit filename_copy, it may also
    // return a pointer inside of it or an array
    // to a statically allocated array
    char *dname = dirname(filename_copy_d);
    size_t dname_size = strlen(dname);

    // Create directory
    err = 0;
    if(dname_size > 0)
      err = makedirs(dname, dname_size, 0777);
    free(filename_copy_d);
    if(err == -1) {
      free(final_filename);
      return;
    }

    // Write file
    FILE *f = fopen(final_filename, "wb");
    free(final_filename);
    size_t wbytes = fwrite(item->data, sizeof(char), item->dsize, f);
    if(wbytes != item->dsize) {
      printf("dumpFileSystem(): Couldn't write to file, wrote %ld/%d bytes.", wbytes, item->dsize);
      return;
    }
    fclose(f);
  }
}
