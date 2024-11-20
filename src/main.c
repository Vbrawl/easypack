#include <stdio.h>
#include "file_sections.h"
#include <stdlib.h>



int main() {
  char* exe_name = getExecutableName();
  if(exe_name == NULL) {
    printf("Name: NULL\n");
  }
  else {
    printf("Name: %s\n", exe_name);
    free(exe_name);
  }

  uint32_t exe_size = getExecutableSize();
  printf("Size: %u", exe_size);

  return 0;
}
