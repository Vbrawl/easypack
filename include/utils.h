#ifndef EASYPACK_UTILS_H
#define EASYPACK_UTILS_H

#include "compatibility-layer.h"
#include "string_array.h"
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>

#define DT_DIR FILE_ATTRIBUTE_DIRECTORY
#define DT_REG 0

#define PLATFORM_PATH_SEPARATOR "\\"

API char* strndup(const char* s, size_t n);
#else

#define API
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#define PLATFORM_PATH_SEPARATOR "/"
#endif

//API int changeDirectory(const char* path);

/**
 * Check if a file exists
 * 
 * @param[in] name  The filename to check
 * 
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
API int checkFileExists(const char *name);

/**
 * A polyfill for memrchr
 *
 * @param[in] s     The string to search in
 * @param[in] c     The character to search for
 * @param[in] n     The length of the string
 *
 * @returns A pointer to the character found on success, NULL on failure
 */
API void* memchr_reverse(const void *s, int c, size_t n);

/**
 * Get the size of a file
 *
 * @param[in] name    The name of the file to get the size
 *
 * @returns The size of the file on success, (size_t)-1 on failure.
 */
API size_t getFileSize(const char *name);

/**
 * Get current working directory
 *
 * @note The caller is responsible for `free`ing the returned pointer.
 *
 * @returns A pointer to the CWD on success, NULL on failure.
 */
API char* getCurrentWorkingDirectory();

/**
 * Create a directory
 *
 * @param[in] path      The directory path
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
API int makeDirectory(const char *path);

/**
 * Recursively create directories.
 *
 * @param[in] path      The path of directories to create
 * @param[in] pathsize  The size of path parameter
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
API int makedirs(const char *path, size_t pathsize);

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
API int listDirectory(const char *dirpath, struct sarray *arr, unsigned char type);

/**
 * Join 2 paths together
 *
 * @param[in] p1        The first path
 * @param[in] p2        The second path
 *
 * @returns A pointer to the result on success, NULL on failure.
 */
API char* pathJoin(const char *p1, const char *p2);

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
API int walkDirectory(const char *root, const char *vroot, struct sarray *arr);


/**
 * Create a temp directory based off of template
 *
 * @note Caller is responsible for `free`ing the returned value
 *
 * @param[in] template  The template of the directory's name
 *
 * @returns A pointer to the actual filename on success, NULL on failure.
 */
API char* make_temp_directory(const char *template);

/**
 * Split data once at the first found separator
 *
 * @param[in] data      The data to split
 * @param[in] dsize     The data size
 * @param[out] part1    An address to a char* variable to return first part
 * @param[out] part2    An address to a char* variable to return second part
 * @param[in] separator The separator
 * @param[in] reverse   Wether to search in normal or reverse order
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
API int splitOnce(const char *data, size_t dsize, char **part1, char **part2, char separator, bool reverse);
#endif
