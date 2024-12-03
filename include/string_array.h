#ifndef EASYPACK_STRING_ARRAY_H
#define EASYPACK_STRING_ARRAY_H

#include <stddef.h>

/**
 * String array
 */
struct sarray {
  char *values;     /// String values
  size_t count;     /// count of strings
  size_t buf_size;  /// Buffer size (aka, size of all strings + NULL bytes)
};

/**
 * Get a reference to the string inside the string array
 *
 * @param this  The sarray object to operate on
 * @param index The index of the string
 *
 * @returns Pointer to the string inside the sarray's buffer on success, NULL on failure
 */
char* sarray_getString(struct sarray *this, size_t index);

/**
 *
 *
 */
int sarray_extendBy(struct sarray *this, size_t additional_size);
int sarray_addString(struct sarray *this, char *string, size_t string_size);
int sarray_extendWith(struct sarray *this, struct sarray *other);
int sarray_clearAll(struct sarray *this);


#endif
