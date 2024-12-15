#ifndef EASYPACK_DYNAMIC_STRING_H
#define EASYPACK_DYNAMIC_STRING_H

#include <stddef.h>

struct dstring {
  char *buf;
  size_t size;
};

int dstring_addString(struct dstring *this, const char *str, size_t str_size);
//int dstring_prependString(struct dstring *this, const char *str, size_t str_size);
void dstring_clearAll(struct dstring *this);

#endif
