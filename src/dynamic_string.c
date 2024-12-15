#include "dynamic_string.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

int dstring_addString(struct dstring *this, const char *str, size_t str_size) {
  char *temp = realloc(this->buf, this->size + str_size + 1);
  if(temp == NULL) return -1;
  this->buf = temp;

  memcpy(this->buf + this->size, str, str_size);
  this->size += str_size;
  this->buf[this->size] = '\0';

  return 0;
}

/*int dstring_prependString(struct dstring *this, const char *str, size_t str_size) {
  char *temp = malloc(this->size + str_size + 1);
  if(temp == NULL) return -1;

  memcpy(temp, str, str_size);
  memcpy(temp + str_size, this->buf, this->size);
  temp[this->size + str_size] = '\0';

  free(this->buf);

  this->buf = temp;
  this->size += str_size;
  return 0;
}*/


void dstring_clearAll(struct dstring *this) {
  if(this->buf != NULL && this->size != 0) free(this->buf);
}
