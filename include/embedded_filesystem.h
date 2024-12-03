#ifndef EASYPACK_EMBEDDED_FILESYSTEM_H
#define EASYPACK_EMBEDDED_FILESYSTEM_H

#include <stdint.h>
#include <sys/types.h>

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
struct fs* loadFileSystemFromData(char* data);

/**
 * Calculate the length of the serialized format of the filesystem.
 *
 * @param[in] system  The system to calculate
 *
 * @returns The length of the serialized data.
 */
uint32_t calculateFileSystemAsDataLength(struct fs *system);

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
char* exportFileSystemAsData(struct fs *system, uint32_t size);

/**
 * Unload a filesystem and clear all memory allocations.
 *
 * @param[in, out] system Clear the given filesystem.
 */
void unLoadFileSystem(struct fs *system);

/**
 * Dump the embedded filesystem to the real filesystem of the OS
 *
 * @param[in] system    The filesystem to dump
 * @param[in] dir_name  The directory to dump all contents in
 */
void dumpFileSystem(struct fs *system, const char* dir_name);

/**
 * Load a real filesystem as an embedded one
 *
 * @note Caller is responsible for `free`ing the returned pointer
 *
 * @param[in] dir_name  The directory to load
 *
 * @returns The loaded embedded filesystem
 */
struct fs* loadFileSystem(const char* dir_name);

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
int addFileToFileSystem(struct fs *system, const char* filename, char* data, off_t dsize);


#endif
