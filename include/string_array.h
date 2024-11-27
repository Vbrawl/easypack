#ifndef EASYPACK_STRING_ARRAY_H
#define EASYPACK_STRING_ARRAY_H

#include <stddef.h>

struct sarray {
  char *values;
  size_t count;
  size_t buf_size;
};

char* sarray_getString(struct sarray *this, size_t index);
int sarray_extendBy(struct sarray *this, size_t additional_size);
int sarray_addString(struct sarray *this, char *string, size_t string_size);
int sarray_extendWith(struct sarray *this, struct sarray *other);
int sarray_clearAll(struct sarray *this);


#endif
