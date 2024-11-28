#ifndef EASYPACK_UTILS_H
#define EASYPACK_UTILS_H

#include <sys/stat.h>
#include <stddef.h>
#include "string_array.h"

int makedirs(char *path, size_t pathsize, mode_t mode);
int listDirectory(const char *dirpath, struct sarray *arr, unsigned char type);
int pathJoin(const char *p1, const char *p2, char **result);
int walkDirectory(const char *root, const char *vroot, struct sarray *arr);

#endif
