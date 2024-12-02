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

/**
 * List all items of a specific type inside a specific directory
 *
 * @param[in] dirpath   The path to the directory to search
 * @param[out] arr      A sarray to fill with all "type" items of the directory
 * @param[in] type      The type to search for in the directory
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
int listDirectory(const char *dirpath, struct sarray *arr, unsigned char type);

/**
 * Join 2 paths together
 *
 * @param[in] p1        The first path
 * @param[in] p2        The second path
 *
 * @returns A pointer to the result on success, NULL on failure.
 */
char* pathJoin(const char *p1, const char *p2);
int walkDirectory(const char *root, const char *vroot, struct sarray *arr);

#endif
