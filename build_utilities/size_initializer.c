#include <stdio.h>
#include <stdint.h>


int main(int argc, const char* argv[]) {
  uint32_t initial_size = 0;

  if(argc <= 1) {
    printf("size_initializer <file>");
    return -1;
  }

  // Open and write
  FILE *f = fopen(argv[1], "ab");
  fwrite(&initial_size, sizeof(initial_size), 1, f);
  fclose(f);

  return 0;
}
