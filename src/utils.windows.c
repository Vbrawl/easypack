#include "utils.h"
#include <windows.h>
#include <stdio.h>

API char* strndup(const char* s, size_t n) {
  char* new_s = malloc(n + 1);
  memcpy(new_s, s, n);
  new_s[n] = '\0';
  return new_s;
}

API size_t getFileSize(const char* name) {
  WIN32_FILE_ATTRIBUTE_DATA info;
  DWORD error = 0;
  size_t result = 0;

  if (GetFileAttributesEx(name, GetFileExInfoStandard, &info) == 0) {
    error = GetLastError();
    printf("getFileSize(): %lu\n", error);
    return -1;
  }

  // Since we want compatibility with 32-bit systems
  // we won't implement 64-bit specific versions.
  // We can worry about that later.
  if (info.nFileSizeHigh > 0) {
    printf("getFileSize(): File too big\n");
    return -1;
  }
  result = info.nFileSizeLow;

  return result;
}

API char* getCurrentWorkingDirectory() {
  char* data = malloc(MAX_PATH);
  if (data == NULL) return NULL;

  if (GetCurrentDirectory(MAX_PATH, data) == 0) {
    DWORD error = GetLastError();
    printf("getCurrentWorkingDirectory(): %lu\n", error);
    free(data);
    return NULL;
  }
  return data;
}

API int makeDirectory(const char* path) {
  int err = CreateDirectory(path, NULL);
  if (err == 0 && GetLastError() != ERROR_ALREADY_EXISTS) return -1;
  return 0;
}

API int listDirectory(const char* dirpath, struct sarray* arr, unsigned char type) {
  WIN32_FIND_DATA data;
  HANDLE hFind;

  char* search_str = pathJoin(dirpath, "*");

  sarray_clearAll(arr);

  hFind = FindFirstFile(search_str, &data);
  if (hFind == INVALID_HANDLE_VALUE) {
    DWORD error = GetLastError();
    printf("listDirectory(): %lu\n", error);
    return -1;
  }

  do {
    if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != type) continue;
    if (data.cFileName[0] == '.' && data.cFileName[1] == '\0') continue;
    if (data.cFileName[0] == '.' && data.cFileName[1] == '.' && data.cFileName[2] == '\0') continue;

    if (sarray_addString(arr, data.cFileName, strlen(data.cFileName)) != 0) {
      sarray_clearAll(arr);
      FindClose(hFind);
      return -1;
    }
  } while (FindNextFile(hFind, &data) != 0);

  FindClose(hFind);

  return 0;
}

API char* make_temp_directory(const char* template) {
  size_t template_len = strlen(template);
  RPC_CSTR uuid_str = NULL;
  size_t uuid_str_len = 0;
  UUID uuid;
  UuidCreate(&uuid);
  UuidToString(&uuid, &uuid_str);
  uuid_str_len = strlen(uuid_str);

  char* malloc_template = malloc(template_len + uuid_str_len + 1);
  if (malloc_template == NULL) return NULL;
  memcpy(malloc_template, template, template_len);
  memcpy(malloc_template + template_len, uuid_str, uuid_str_len);
  malloc_template[template_len + uuid_str_len] = '\0';
  RpcStringFree(&uuid_str);

  if (CreateDirectory(malloc_template, NULL) == 0 && GetLastError() != ERROR_ALREADY_EXISTS) {
    perror("make_temp_directory");
    free(malloc_template);
    return NULL;
  }

  return malloc_template;
}