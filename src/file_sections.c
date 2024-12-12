#include "file_sections.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h>
#include <string.h>

API uint32_t getExecutableSize(const char *exe_name) {
  uint32_t total_size = 0, data_size = 0;

  total_size = (uint32_t)getFileSize(exe_name);
  data_size = getEmbeddedDataSize(exe_name);

  return total_size - data_size - sizeof(data_size);
}

API uint32_t getEmbeddedDataSize(const char* exe_name) {
  uint32_t results = 0;
  FILE *f = NULL;
  size_t rbytes = 0;

  // Open file
  f = fopen(exe_name, "rb");
  if(f == NULL) {
    perror("getEmbeddedDataSize()");
    return 0;
  }

  // Read last 4 bytes
  fseek(f, -sizeof(uint32_t), SEEK_END);
  rbytes = fread(&results, sizeof(uint32_t), 1, f);
  if(rbytes != 1) {
    printf("getEmbeddedDataSize(): Couldn't read embedded size, only %ld bytes read!\n", rbytes);
    return 0;
  }

  // Clean up
  fclose(f);
  return results;
}

API char* getEmbeddedData(const char* exe_name) {
  FILE *f = NULL;
  char* data = NULL;
  size_t rbytes = 0;

  uint32_t data_size = getEmbeddedDataSize(exe_name);
  if(data_size == 0) {
    return NULL;
  }

  data = malloc(data_size + 1);
  if(data == NULL) {
    perror("getEmbeddedData()");
    return NULL;
  }

  f = fopen(exe_name, "rb");
  if(f == NULL) {
    perror("getEmbeddedData()");
    free(data);
    return NULL;
  }

  fseek(f, -(data_size + sizeof(uint32_t)), SEEK_END);
  rbytes = fread(data, sizeof(char), data_size, f);
  if(rbytes != data_size) {
    printf("getEmbeddedData(): Couldn't read embedded data, %ld/%d bytes read!\n", rbytes, data_size);
    free(data);
    return NULL;
  }
  data[rbytes] = '\0';

  fclose(f);
  return data;
}


API void setEmbeddedData(const char* exe_name, const char* new_exe_name, char* data, uint32_t dsize) {
  FILE *f = NULL;
  size_t wbytes = 0, rbytes = 0;

  uint32_t esize = getExecutableSize(exe_name);

  uint32_t final_size = esize + dsize + sizeof(uint32_t);
  char* final_data = malloc(final_size);
  if(final_data == NULL) {
    printf("setEmbeddedData(): Couldn't allocate memory!\n");
    return;
  }

  // Read exe
  f = fopen(exe_name, "rb");
  rbytes = fread(final_data, sizeof(char), esize, f);
  fclose(f);
  if(rbytes != esize) {
    printf("setEmbeddedData(): Couldn't read executable data, %ld/%d bytes read!\n", rbytes, esize);
    free(final_data);
    return;
  }

  // Prepare contents
  memcpy(final_data + esize, data, dsize);
  memcpy(final_data + esize + dsize, &dsize, sizeof(dsize));

  // Write new exe
  f = fopen(new_exe_name, "wb");
  wbytes = fwrite(final_data, sizeof(char), final_size, f);
  fclose(f);
  free(final_data);
  if(wbytes != final_size) {
    printf("setEmbeddedData(): Couldn't rewrite the exe file, %ld/%d bytes written!\n", wbytes, final_size);
    return;
  }
}
