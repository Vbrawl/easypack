#include "string_array.h"
#include <string.h>
#include <stdlib.h>


char* sarray_getString(struct sarray *this, size_t index) {
  if(index >= this->count) return NULL;
  char *cursor = this->values;

  for(size_t i = 0; i < index; i++) {
    size_t offset = strlen(cursor);
    cursor += offset;
  }

  return cursor;
}

int sarray_extendBy(struct sarray *this, size_t additional_size) {
  char *temp = realloc(this->values, this->buf_size + additional_size);
  if(temp == NULL) return -1;

  this->buf_size += additional_size;
  this->values = temp;
  return 0;
}

int sarray_addString(struct sarray *this, char *string, size_t string_size) {
  size_t old_buf_size = this->buf_size;

  int err = sarray_extendBy(this, string_size);
  if(err != 0) return err;

  memcpy(this->values + old_buf_size, string, string_size);
  this->values[this->buf_size] = '\0';
  return 0;
}

int sarray_extendWith(struct sarray *this, struct sarray *other) {
  size_t old_buf_size = this->buf_size;
  size_t item_size = 0;
  char *item = NULL;
  size_t i = 0;
  int err = 0;

  err = sarray_extendBy(this, other->buf_size);
  if(err != 0) return err;

  memcpy(this->values + old_buf_size, other->values, other->buf_size);
  this->count += other->count;

  return 0;
}

int sarray_clearAll(struct sarray *this) {
  if(this->values) free(this->values);
  this->values = NULL;
  this->count = 0;
  this->buf_size = 0;
}
