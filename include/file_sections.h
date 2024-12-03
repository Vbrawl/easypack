#ifndef EASYPACK_FILE_SECTIONS_H
#define EASYPACK_FILE_SECTIONS_H

#include <stdint.h>
#define MAX_FILENAME_SIZE 1024

/**
 * Get the name of this program's executable.
 *
 * @note  The caller is responsible for `free`ing the allocated string.
 *
 * @returns A pointer to a string containing the executable's name, or NULL on failure.
 */
char* getExecutableName();

/**
 * Get size of the embedded data
 *
 * see Documentation/EXECUTABLE_STRUCTURE.md
 *
 * Args:
 *    exe_name: Executable's name or NULL for automatic resolution
 *
 * Returns:
 *    Embedded data size
 */
uint32_t getEmbeddedDataSize(char* exe_name);
uint32_t getExecutableSize();


char* getEmbeddedData(char* exe_name);

void setEmbeddedData(const char* exe_name, const char* new_exe_name, char* data, uint32_t dsize);


#endif
