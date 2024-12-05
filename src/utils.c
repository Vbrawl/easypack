#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>


int makedirs(const char *path, size_t pathsize, mode_t mode) {
  char *my_path = NULL;
  size_t components = 1, pathlen = 0, i = 0;
  bool throw_error = 0;

  // Avoid creating current, previous and root directories
  if(pathsize == 1 && path[0] == '/') return 0;
  if(pathsize == 1 && path[0] == '.') return 0;
  if(pathsize == 2 && path[0] == '.' && path[1] == '.') return 0;

  // Get a copy of path
  my_path = strndup(path, pathsize);
  if(my_path == NULL) return -1;

  // Replace all "/" with NULL-byte, only skip the first
  // Notice "components" variable's value
  for(i = 1; i < pathsize; i++) {
    // Don't edit escaped slashes
    if(my_path[i-1] == '\\') continue;
    if(my_path[i] == '/') {
      my_path[i] = '\0';
      components++;
    }
  }

  // Create component and recover slash
  for(i = 0; i < components; i++) {
    if(mkdir(my_path, mode) == -1) {
      throw_error = (errno != EEXIST);
      if(throw_error) {
        perror("makedirs()");
        free(my_path);
        return -1;
      }
    }

    pathlen = strlen(my_path);
    my_path[pathlen] = '/';
  }

  // Cleanup
  free(my_path);
  return 0;
}


int listDirectory(const char* dirpath, struct sarray *arr, unsigned char type) {
  DIR *d = NULL;
  struct dirent *dentry = NULL;
  char *temp = NULL;
  size_t fname_size = 0;

  sarray_clearAll(arr);

  // Open directory
  d = opendir(dirpath);
  if(d == NULL) {
    perror("listDirectory()");
    return -1;
  }

  // Read all entries in the directory
  while((dentry = readdir(d)) != NULL) {
    if(dentry->d_type != type) continue;
    if(dentry->d_name[0] == '.' && dentry->d_name[1] == '\0') continue;
    if(dentry->d_name[0] == '.' && dentry->d_name[1] == '.'
       && dentry->d_name[2] == '\0') continue;
    // Runs only for required type

    // get name size
    fname_size = strlen(dentry->d_name);

    // add name to sarray
    if(0 != sarray_addString(arr, dentry->d_name, fname_size)) {
      perror("listDirectory()");
      sarray_clearAll(arr);
      closedir(d);
      return -1;
    }
  }

  // Cleanup
  closedir(d);
  return 0;
}

char* pathJoin(const char *p1, const char *p2) {
  char *result = NULL;
  size_t result_size = 0;

  // Return error if all paths are NULL
  if(p1 == NULL && p2 == NULL) return NULL;

  // Get expected size of the result (with optimizations)
  if(p1 == NULL) result_size = strlen(p2);
  else if(p2 == NULL) result_size = strlen(p1);
  else result_size = snprintf(NULL, 0, "%s/%s", p1, p2);

  // Allocate memory for result
  result = malloc(result_size + 1);
  if(result == NULL) return NULL;

  // Construct result's value
  if(p1 == NULL) memcpy(result, p2, result_size);
  else if(p2 == NULL) memcpy(result, p1, result_size);
  else snprintf(result, result_size + 1, "%s/%s", p1, p2);

  result[result_size] = '\0';
  return result;
}

int walkDirectory(const char *root, const char *vroot, struct sarray *arr) {
  /* Prepare and initialize variables */
  sarray_clearAll(arr); // Results array
  struct sarray filenames = {0}; // Temporarily store all filenames of root
  struct sarray nested_results = {0}; // Temporarily store all results of nested walkDirectory()
  char *filename = NULL; // Store each iteration's filename in the below loops
  char *result = NULL; // Store each iteration's fullpath in the below loops
  char *vresult = NULL; // Store each iteration's virtual fullpath in the below loops
  size_t i = 0; // Iterator for loops
  int err = 0;

  /* List files in root */
  err = listDirectory(root, &filenames, DT_REG);
  if(err != 0) return -1;

  /* Add fullpath of files to "arr" string array */
  for(i = 0; i < filenames.count; i++) {
    filename = sarray_getString(&filenames, i);
    vresult = pathJoin(vroot, filename);
    if(vresult == NULL) {
      sarray_clearAll(&filenames);
      return -1;
    }

    sarray_addString(arr, vresult, strlen(vresult));

    // Cleanup pathJoin's memory allocation
    free(vresult);
  }

  sarray_clearAll(&filenames);
  err = listDirectory(root, &filenames, DT_DIR);
  if(err != 0) return -1;

  for(i = 0; i < filenames.count; i++) {
    filename = sarray_getString(&filenames, i);
    vresult = pathJoin(vroot, filename);
    if(vresult == NULL) return -1;

    result = pathJoin(root, filename);
    if(result == NULL) {
      free(vresult);
      return -1;
    }

    // Recursive call
    err = walkDirectory(result, vresult, &nested_results);
    if(err != 0) {
      sarray_clearAll(&filenames);
      free(vresult);
      free(result);
      return -1;
    }

    // Connect results together
    sarray_extendWith(arr, &nested_results);

    // Cleanup pathJoin's and walkDirectory's memory allocations
    sarray_clearAll(&nested_results);
    free(result);
    free(vresult);
  }

  return 0;
}

char* make_temp_directory(const char *template) {
  size_t template_len = strlen(template);
  size_t X_append = 6;

  char *malloc_template = malloc(template_len + X_append + 1);
  if(malloc_template == NULL) return NULL;
  strncpy(malloc_template, template, template_len);
  memset(malloc_template + template_len, 'X', X_append);
  malloc_template[template_len + X_append] = '\0';

  if(mkdtemp(malloc_template) == NULL) {
    perror("mkdtemp");
    free(malloc_template);
    return NULL;
  }

  return malloc_template;
}

