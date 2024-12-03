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
 * Extend the internal sarray's buffer by a requested nubmer of bytes
 *
 * @param this            The sarray object to operate on
 * @param additional_size The number of bytes to add to the internal buffer's length
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 *
 */
int sarray_extendBy(struct sarray *this, size_t additional_size);

/**
 * Add a copy of the given string to the sarray
 * @note  A NULL-byte is automatically added to sarray's copy
 *
 * @bug Setting `string_size` to a value larger than `string`'s
 *      length is undefined behaviour.
 *
 * @bug Adding a `string` that contains NULL-bytes in undefined
 *      behaviour
 *
 * @param this        The sarray object to operate on
 * @param string      The string to copy to the sarray
 * @param string_size The size of the string
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
int sarray_addString(struct sarray *this, char *string, size_t string_size);

/**
 * Extend sarray with another sarray
 *
 *
 * @param this  The sarray object to extend
 * @param other The sarray object to be appended
 *
 * @retval 0  Succeeded
 * @retval -1 Failed
 */
int sarray_extendWith(struct sarray *this, struct sarray *other);

/**
 * Clear all fields of a sarray and free
 * all allocated memory
 */
void sarray_clearAll(struct sarray *this);


#endif
