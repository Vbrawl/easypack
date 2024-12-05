#include "string_array.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


char* sarray_getString(struct sarray *this, size_t index) {
  // Check if index exists
  if(index >= this->count) return NULL;
  char *cursor = this->values;

  // Move the cursor to the requested string's position
  for(size_t i = 0; i < index; i++) {
    size_t offset = strlen(cursor);
    cursor += offset + 1;
  }

  // Return cursor
  return cursor;
}

char* sarray_getNextString(struct sarray *this, char *cursor) {
  if(cursor == NULL) return this->values;

  size_t cursor_offset = cursor - this->values;
  size_t cursor_len = strlen(cursor);

  if(cursor_offset + cursor_len + 1 >= this->buf_size) return NULL;

  cursor += cursor_len + 1;
  return cursor;
}

int __sarray_addStringFromPointers(struct sarray *this, const char *start, size_t len) {
  char *buf = NULL;
  int err = 0;

  buf = malloc(len + 1);
  if(buf == NULL) {
    perror("__sarray_addStringFromPointers -> malloc");
    return -1;
  }

  strncpy(buf, start, len);
  buf[len] = '\0';

  err = sarray_addString(this, buf, len);
  free(buf);

  if(err == 0)  return 0;
  else          return -1;
}

int sarray_addStringsFromList(struct sarray *this, const char *list, char separator) {
  char *buf = NULL;
  const char *start, *end, *list_end;
  size_t buf_len = 0;
  int err = 0;
  list_end = list + strlen(list);
  start = end = list;

  while(end < list_end) {
    if(*end == separator) {
      buf_len = end - start;
      err = __sarray_addStringFromPointers(this, start, buf_len);
      if(err != 0) return -1;
      start = end + 1;
    }
    end++;
  }

  if(start < end) {
    buf_len = end - start;
    err = __sarray_addStringFromPointers(this, start, buf_len);
    if(err != 0) return -1;
    start = end + 1;
  }

  return 0;
}

int sarray_extendBy(struct sarray *this, size_t additional_size) {
  char *temp = realloc(this->values, this->buf_size + additional_size);
  if(temp == NULL) return -1;

  this->buf_size += additional_size;
  this->values = temp;
  return 0;
}

int sarray_addString(struct sarray *this, const char *string, size_t string_size) {
  size_t old_buf_size = this->buf_size;

  int err = sarray_extendBy(this, string_size + 1);
  if(err != 0) return -1;

  memcpy(this->values + old_buf_size, string, string_size);
  this->values[this->buf_size - 1] = '\0';

  this->count++;
  return 0;
}

int sarray_extendWith(struct sarray *this, struct sarray *other) {
  size_t old_buf_size = this->buf_size;
  size_t item_size = 0;
  char *item = NULL;
  size_t i = 0;
  int err = 0;

  err = sarray_extendBy(this, other->buf_size);
  if(err != 0) return -1;

  memcpy(this->values + old_buf_size, other->values, other->buf_size);
  this->count += other->count;

  return 0;
}

void sarray_clearAll(struct sarray *this) {
  if(this->values) free(this->values);
  this->values = NULL;
  this->count = 0;
  this->buf_size = 0;
}
