#include "embedded_filesystem.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


API struct fs* loadFileSystemFromData(char* data) {
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

API void unLoadFileSystem(struct fs *system) {
  for(uint32_t i = 0; i < system->size; i++) {
    struct fs_item *item = &system->files[i];
    free(item->filename);
    free(item->data);
  }

  free(system->files);
  free(system);
}

API uint32_t calculateFileSystemAsDataLength(struct fs *system) {
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

API char* exportFileSystemAsData(struct fs *system, uint32_t size) {
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

API void dumpFileSystem(struct fs *system, const char* dir_name) {
  size_t i = 0, dname_size = 0, wbytes = 0;
  int err = 0;
  struct fs_item *item = NULL;
  FILE *f = NULL;

  char *fullpath = NULL;
  char *dname = NULL, *bname = NULL;

  for(i = 0; i < system->size; i++) {
    item = &system->files[i];

    // Combine filename with dir_name
    if((fullpath = pathJoin(dir_name, item->filename)) == NULL) return;

    splitOnce(fullpath, strlen(fullpath), &dname, &bname, PLATFORM_PATH_SEPARATOR[0], true);
    dname_size = strlen(dname);

    // Create directory
    err = 0;
    if(dname_size > 0) err = makedirs(dname, dname_size);
    free(dname);
    free(bname);

    if(err != 0) {
      free(fullpath);
      return;
    }

    // Write file
    f = fopen(fullpath, "wb");
    free(fullpath);
    if(f == NULL) {
      perror("dumpFileSystem()");
      return;
    }

    wbytes = fwrite(item->data, sizeof(char), item->dsize, f);
    fclose(f);
    if(wbytes != item->dsize) {
      printf("dumpFileSystem(): Couldn't write to file, wrote %zd/%d bytes.", wbytes, item->dsize);
      return;
    }
  }
}


API struct fs* loadFileSystem(const char* dir_name) {
  int err = 0;
  size_t i = 0, rbytes = 0, filesize = 0;
  struct sarray files = {0};
  const char *filename = NULL;
  char *fullpath = NULL;
  char *data = NULL;
  FILE *f = NULL;
  struct fs *system = malloc(sizeof(struct fs));
  system->size = 0;
  system->files = NULL;

  walkDirectory(dir_name, NULL, &files);
  if(files.count == 0) {
    unLoadFileSystem(system);
    sarray_clearAll(&files);
    return NULL;
  }

  for(i = 0; i < files.count; i++) {
    filename = sarray_getString(&files, i);
    fullpath = pathJoin(dir_name, filename);
    filesize = getFileSize(fullpath);

    data = malloc(sizeof(char) * filesize);
    if(data == NULL) {
      perror("loadFileSystem()");
      free(fullpath);
      unLoadFileSystem(system);
      sarray_clearAll(&files);
      return NULL;
    }

    f = fopen(fullpath, "rb");
    if(f == NULL) {
      perror("loadFileSystem()");
      free(data);
      free(fullpath);
      unLoadFileSystem(system);
      sarray_clearAll(&files);
      return NULL;
    }
    free(fullpath);
    rbytes = fread(data, sizeof(char), filesize, f);
    fclose(f);

    if(rbytes != (size_t)filesize) {
      printf("loadFileSystem(): Couldn't read whole file.\n");
      free(data);
      unLoadFileSystem(system);
      sarray_clearAll(&files);
      return NULL;
    }

    err = addFileToFileSystem(system, filename, data, filesize);
    free(data);
    if(err != 0) {
      unLoadFileSystem(system);
      sarray_clearAll(&files);
      return NULL;
    }
  }

  return system;
}

API int addFileToFileSystem(struct fs *system, const char* filename, char* data, size_t dsize) {
  if(dsize > UINT32_MAX) {
    printf("File can't be added, too big!\n");
    return -1;
  }
  uint32_t dsize32 = (dsize & UINT32_MAX);

  struct fs_item *temp_files = realloc(system->files, sizeof(struct fs_item) * (system->size + 1));
  if(temp_files == NULL) {
    perror("addFileToFileSystem()");
    return -1;
  }
  system->files = temp_files;

  struct fs_item *item = &system->files[system->size++];

  // set fsize and filename
  item->fsize = (uint32_t)strlen(filename);
  item->filename = malloc(item->fsize + 1);
  memcpy(item->filename, filename, sizeof(char) * item->fsize);
  item->filename[sizeof(char) * item->fsize] = '\0';

  // set dsize and data
  item->dsize = dsize32;
  item->data = malloc(sizeof(char) * item->dsize);
  memcpy(item->data, data, sizeof(char) * item->dsize);

  return 0;
}


API int extendFileSystem(struct fs *system, struct fs *other, const char *vroot) {
  size_t i = 0;
  char *fullpath = NULL;
  struct fs_item *cursor = NULL;

  for(i = 0; i < other->size; i++) {
    cursor = &other->files[i];

    fullpath = pathJoin(vroot, cursor->filename);
    if(fullpath == NULL) return -1;

    addFileToFileSystem(system, fullpath, cursor->data, cursor->dsize);

    free(fullpath);
  }

  return 0;
}

