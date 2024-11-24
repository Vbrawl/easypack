#include <stdio.h>
#include "file_sections.h"
#include "embedded_filesystem.h"
#include <stdlib.h>
#include <string.h>


int main() {
  char* exe_name = getExecutableName();
  if(exe_name == NULL) {
    printf("Couldn't resolve executable name!\n");
    return 1;
  }
  printf("Name: %s\n", exe_name);

  uint32_t exe_size = getExecutableSize();
  printf("Executable Size: %u\n", exe_size);

  uint32_t data_size = getEmbeddedDataSize(NULL);
  printf("Data Size: %u\n", data_size);

  char* data = getEmbeddedData(NULL);
  if(data == NULL) {
    printf("Data: NULL\n");
    printf("Adding fake data!\n");

    struct fs *system = malloc(sizeof(struct fs));
    system->size = 1;
    system->files = malloc(sizeof(struct fs_item));
    system->files[0].filename = "Hello World";
    system->files[0].fsize = 12;
    system->files[0].data = "awd";
    system->files[0].dsize = 4;

    uint32_t fake_size = calculateFileSystemAsDataLength(system);
    char* fake_data = exportFileSystemAsData(system, fake_size);
    printf("%s\n", fake_data);

    size_t namesize = strlen(exe_name);
    char* new_name = malloc(namesize + 5);
    memcpy(new_name, exe_name, strlen(exe_name));
    new_name[namesize] = '.';
    new_name[namesize + 1] = 'n';
    new_name[namesize + 2] = 'e';
    new_name[namesize + 3] = 'w';
    new_name[namesize + 4] = '\0';

    setEmbeddedData(exe_name, new_name, fake_data, system->size);

    free(new_name);
    free(system->files);
    free(system);
    free(fake_data);
    free(exe_name);
    return 0;
  }
  else {
    printf("Data: %s\n", data);
  }

  struct fs *system = loadFileSystemFromData(data);

  printf("FS Items: %d\n", system->size);
  for(uint32_t i = 0; i < system->size; i++) {
    char* temp_data = malloc(system->files[i].fsize + 1);
    memcpy(temp_data, system->files[i].filename, system->files[i].fsize);
    data[system->files[i].fsize] = '\0';

    printf("File %d: %s\n", system->files[i].fsize, temp_data);

    free(temp_data);
  }

  unLoadFileSystem(system);
  free(data);
  free(exe_name);
  return 0;
}
