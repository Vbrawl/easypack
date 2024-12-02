#ifndef EASYPACK_UTILS_H
#define EASYPACK_UTILS_H

#include <sys/stat.h>
#include <stddef.h>
#include "string_array.h"

/**
 * Recursively create directories.
 *
 * @param[in] path      The path of directories to create
 * @param[in] pathsize  The size of path parameter
 * @param[in] mode      The mode to use to create the new directories
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
int makedirs(const char *path, size_t pathsize, mode_t mode);
int listDirectory(const char *dirpath, struct sarray *arr, unsigned char type);
int pathJoin(const char *p1, const char *p2, char **result);
int walkDirectory(const char *root, const char *vroot, struct sarray *arr);

#endif
