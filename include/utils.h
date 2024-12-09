#ifndef EASYPACK_UTILS_H
#define EASYPACK_UTILS_H

#include <sys/stat.h>
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

/**
 * Get all files in a directorie's sub-tree
 *
 * @warning RECURSIVE IMPLEMENTATION
 *
 * @param[in] root      The root directory of the tree to search
 * @param[in] vroot     The path to prepend to the results
 * @param[out] arr      An array to store the results
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
int walkDirectory(const char *root, const char *vroot, struct sarray *arr);


/**
 * Create a temp directory based off of template
 *
 * @note Caller is responsible for `free`ing the returned value
 *
 * @param[in] template  The template of the directory's name
 *
 * @returns A pointer to the actual filename on success, NULL on failure.
 */
char* make_temp_directory(const char *template);

/**
 * Split data once at the first found separator
 *
 * @param[in] data      The data to split
 * @param[in] dsize     The data size
 * @param[out] part1    An address to a char* variable to return first part
 * @param[out] part2    An address to a char* variable to return second part
 * @param[in] separator The separator
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
int splitOnce(const char *data, size_t dsize, char **part1, char **part2, char separator);

#endif
