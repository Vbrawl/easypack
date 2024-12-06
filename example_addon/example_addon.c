#include <stdio.h>
#include <easypack_addon.h>
#include <string.h>

int execute(struct fs *system) {
  const char *filename = "example.txt";
  char *data = "My Data\n";
  off_t dsize = strlen(data);

  printf("Executing example addon!\n");
  addFileToFileSystem(system, filename, data, dsize);
  printf("Example addon added \"%s\" to the package.\n", filename);
  return 0;
}
