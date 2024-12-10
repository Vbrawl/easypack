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
 * @param exe_name Executable's name
 *
 * @returns Embedded data size
 */
uint32_t getEmbeddedDataSize(const char* exe_name);

/**
 * Get size of the executable
 *
 * @param[in] exe_name  The executable's name
 *
 * @returns The size of the executable on success, NULL on failure.
 */
uint32_t getExecutableSize(const char *exe_name);


/**
 * Get embedded data from the specified executable
 *
 * @param exe_name The executable's name to read
 *
 * @returns A pointer to the embedded executable's data.
 */
char* getEmbeddedData(const char* exe_name);

/**
 * Embbed additional data on an executable
 *
 * @param[in] exe_name      The executable's name to get code from (should be current executable's name)
 * @param[in] new_exe_name  The name of the output file
 * @param[in] data          The data to embbed to the output executable file
 * @param[in] dsize         The data size
 */
void setEmbeddedData(const char* exe_name, const char* new_exe_name, char* data, uint32_t dsize);


#endif
