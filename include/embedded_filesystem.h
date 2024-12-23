#ifndef EASYPACK_EMBEDDED_FILESYSTEM_H
#define EASYPACK_EMBEDDED_FILESYSTEM_H

#include <stdint.h>
#include <stddef.h>

#include "compatibility-layer.h"

/**
 * Embedded Filesystem item
 *
 * Holds data about a singly file
 * in this custom filesystem.
 */
struct fs_item {
  char *filename;
  char *data;
  uint32_t fsize; /// filename size
  uint32_t dsize; /// data size
};

/**
 * Embedded Filesystem
 */
struct fs {
  struct fs_item *files; /// Array of `fs_item`s
  uint32_t size; /// Number of items in `files`
};

/**
 * Load a filesystem from the given data.
 * Essentially deserialize the data into a structure.
 *
 * @note Caller is responsible for `free`ing the returned pointer.
 *
 * @param[in] data  The data to deserialize
 *
 * @returns The structure generated from data
 */
API struct fs* loadFileSystemFromData(char* data);

/**
 * Calculate the length of the serialized format of the filesystem.
 *
 * @param[in] system  The system to calculate
 *
 * @returns The length of the serialized data.
 */
API uint32_t calculateFileSystemAsDataLength(struct fs *system);

/**
 * Get the serialized version of the filesystem.
 *
 * @note Caller is responsible for `free`ing the returned pointer.
 *
 * @param[in] system  The filesystem to serialize
 * @param[in] size    The filesystem's serialized data length
 *
 * @returns A pointer to the serialized data.
 *
 */
API char* exportFileSystemAsData(struct fs *system, uint32_t size);

/**
 * Unload a filesystem and clear all memory allocations.
 *
 * @param[in, out] system Clear the given filesystem.
 */
API void unLoadFileSystem(struct fs *system);

/**
 * Dump the embedded filesystem to the real filesystem of the OS
 *
 * @param[in] system    The filesystem to dump
 * @param[in] dir_name  The directory to dump all contents in
 */
API void dumpFileSystem(struct fs *system, const char* dir_name);

/**
 * Load a real filesystem as an embedded one
 *
 * @note Caller is responsible for `free`ing the returned pointer
 *
 * @param[in] dir_name  The directory to load
 *
 * @returns The loaded embedded filesystem
 */
API struct fs* loadFileSystem(const char* dir_name);

/**
 * Add a file to the embedded filesystem
 *
 * @param[in, out]  system      The filesystem that will be altered
 * @param[in]       filename    The filename inside the embedded filesystem
 * @param[in]       data        The data of the file
 * @param[in]       dsize       The data size
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
API int addFileToFileSystem(struct fs *system, const char* filename, char* data, size_t dsize);


/**
 * Add a filesystem inside another filesystem struct under a specific virtual root.
 *
 * @param[in, out]  system      The filesystem to be updated
 * @param[in]       other       The filesystem to be added
 * @param[in]       vroot       The directory in which `other` should be placed inside `system`
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
API int extendFileSystem(struct fs *system, struct fs *other, const char *vroot);

/**
 * Remove a file from a filesystem
 *
 * @note Only files can be removed!
 *
 * @param[in, out]  system      The filesystem to be updated
 * @param[in]       filename    The filename to be removed
 *
 * @retval 0  Succeeded (memory usage may or may not be decreased)
 * @retval -1 Failed
 */
API int removeFileFromFileSystem(struct fs *system, const char *filename);


/**
 * Get file by name
 *
 * @param[in]   system    The filesystem to be queried
 *
 * @returns A pointer to the fs_item in the system or NULL on failure
 */
API struct fs_item* getFileFromFileSystem(struct fs *system, const char *filename);
#endif
