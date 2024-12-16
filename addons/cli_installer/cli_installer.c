#include <stdio.h>
#include "embedded_filesystem.h"
#include "dynamic_string.h"
#include "forward.h"
#include <string.h>
#include <stdlib.h>

#define UNUSED(x) (void)(x);

#define CLIINSTALLER_INSTALL_FILENAME "install.clii"
#define CLIINSTALLER_UNINSTALL_FILENAME "uninstall.clii"

#ifdef _WIN32

#else

#define OS_SCRIPT_HEADER "#!/bin/sh\n"

#define OS_COPY_SCRIPT  "cp -r \"${CLII_PACKAGE}%s\" \"${CLII_PREFIX}%s\"\n"
#define OS_DELETE_SCRIPT "rm -r \"${CLII_PREFIX}%s\"\n"
#define OS_MARKEXEC_SCRIPT "chmod +x \"${CLII_PREFIX}%s\"\n"
#define OS_EXEC_SCRIPT "CLII_OLDPWD=$(pwd) && cd ${CLII_PREFIX} && \"%s\" && cd ${CLII_OLDPWD} && unset CLII_OLDPWD\n"
#define OS_DIRECTORY_SCRIPT "mkdir -p \"${CLII_PREFIX}%s\"\n"
#define OS_PRINT_SCRIPT "echo \"%s\"\n"

#define OS_STARTUP_SCRIPT OS_SCRIPT_HEADER \
                          "export CLII_PREFIX=\"${CLII_PREFIX:=\"\"}/\"\n" \
                          "export CLII_BINARY=\"bin/\"\n" \
                          "export CLII_INCLUDE=\"include/\"\n" \
                          "export CLII_LIBRARY=\"lib/\"\n" \
                          "export CLII_PACKAGE=\"$(pwd)/\"\n" \
                          "cd \"${EASYPACK_CWD}\"\n" \
                          "chmod +x \"${CLII_PACKAGE}" CLIINSTALLER_INSTALL_FILENAME "\" 2> /dev/null \n" \
                          "chmod +x \"${CLII_PACKAGE}" CLIINSTALLER_UNINSTALL_FILENAME "\" 2> /dev/null\n" \
                          "if [ \"$1\" == \"uninstall\" ]\n" \
                          "then\n" \
                          "  \"${CLII_PACKAGE}" CLIINSTALLER_UNINSTALL_FILENAME "\"\n" \
                          "else\n" \
                          "  \"${CLII_PACKAGE}" CLIINSTALLER_INSTALL_FILENAME "\"\n" \
                          "fi\n"
#endif

#define SCRIPT_SEPARATOR " -> "
#define SCRIPT_SEPARATOR_LEN strlen(SCRIPT_SEPARATOR)

#define DELETE_SYMBOL '-'
#define MARKEXEC_SYMBOL '+'
#define EXEC_SYMBOL '$'
#define DIRECTORY_SYMBOL '#'
#define PRINT_SYMBOL '%'


enum cmd_code {
  CMDCODE_INVALID = 0,  // Invalid command
  CMDCODE_COPY,         // pkg/src -> inst/dest
  CMDCODE_DELETE,       // inst/src -> -
  CMDCODE_MARKEXEC,     // inst/src -> +
  CMDCODE_EXEC,         // inst/src -> $
  CMDCODE_DIRECTORY,    // inst/src -> #
  CMDCODE_PRINT         // lorem ip -> %
};

struct parser_results {
  enum cmd_code cmd;
  char *arg1;
  char *arg2;
};

char* find_nonspace(char *str) {
  while(*str != '\0' && (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r')) str++;
  return str;
}

char* find_nonspace_reverse(char *str) {
  char *str_end = str + strlen(str) - 1;
  while(str_end != str && (*str_end == ' ' || *str_end == '\0' || *str_end == '\t' || *str_end == '\n' || *str_end == '\r'))
    str_end--;
  return str_end;
}

char* trim_whitespace(const char *str, size_t size) {
  char *s = strndup(str, size);
  char *s_start = find_nonspace(s);
  char *s_end = find_nonspace_reverse(s) + 1;

  char *stripped_s = strndup(s_start, s_end - s_start);
  free(s);

  return stripped_s;
}

enum cmd_code detect_command(char *arg1, char *arg2) {
  UNUSED(arg1);
  size_t arg2_size = strlen(arg2);

  if(arg2_size == 0) return CMDCODE_INVALID;
  if(arg2_size > 1) return CMDCODE_COPY;

  switch(arg2[0]) {
  case DELETE_SYMBOL:     return CMDCODE_DELETE;
  case MARKEXEC_SYMBOL:   return CMDCODE_MARKEXEC;
  case EXEC_SYMBOL:       return CMDCODE_EXEC;
  case DIRECTORY_SYMBOL:  return CMDCODE_DIRECTORY;
  case PRINT_SYMBOL:      return CMDCODE_PRINT;
  }

  return CMDCODE_INVALID;
}

int parse_line(const char *line, struct parser_results *results) {
  size_t line_size = strlen(line);
  char *cursor = strstr(line, SCRIPT_SEPARATOR);

  // Assume comment
  if(cursor == NULL) {
    results->cmd = CMDCODE_INVALID;
    results->arg1 = NULL;
    results->arg2 = NULL;
    return 0;
  }

  // Assume command
  results->arg1 = trim_whitespace(line, cursor - line);
  results->arg2 = trim_whitespace(cursor + SCRIPT_SEPARATOR_LEN, (line + line_size) - cursor);
  results->cmd = detect_command(results->arg1, results->arg2);

  return 0;
}

#define COMPILE_COMMAND(text, ...)  do { \
                                      size_t res_size = snprintf(NULL, 0, text, __VA_ARGS__); \
                                      char *res = malloc(res_size + 1); \
                                      if(res == NULL) return NULL; \
                                      snprintf(res, res_size + 1, text, __VA_ARGS__); \
                                      return res; \
                                    } while(0);

char* command_COPY(const char *arg1, const char *arg2) {COMPILE_COMMAND(OS_COPY_SCRIPT, arg1, arg2);}
char* command_DELETE(const char *arg1) {COMPILE_COMMAND(OS_DELETE_SCRIPT, arg1);}
char* command_MARKEXEC(const char *arg1) {COMPILE_COMMAND(OS_MARKEXEC_SCRIPT, arg1);}
char* command_EXEC(const char *arg1) {COMPILE_COMMAND(OS_EXEC_SCRIPT, arg1);}
char* command_DIRECTORY(const char *arg1) {COMPILE_COMMAND(OS_DIRECTORY_SCRIPT, arg1);}
char* command_PRINT(const char *arg1) {COMPILE_COMMAND(OS_PRINT_SCRIPT, arg1);}

char* exec_from_results(struct parser_results *results) {
  char *str_results = NULL;

  switch(results->cmd) {
  case CMDCODE_COPY:
    str_results = command_COPY(results->arg1, results->arg2);
    break;
  case CMDCODE_DELETE:
    str_results = command_DELETE(results->arg1);
    break;
  case CMDCODE_MARKEXEC:
    str_results = command_MARKEXEC(results->arg1);
    break;
  case CMDCODE_EXEC:
    str_results = command_EXEC(results->arg1);
    break;
  case CMDCODE_DIRECTORY:
    str_results = command_DIRECTORY(results->arg1);
    break;
  case CMDCODE_PRINT:
    str_results = command_PRINT(results->arg1);
    break;
  case CMDCODE_INVALID:
  default:
    str_results = NULL;
    break;
  }

  free(results->arg1);
  free(results->arg2);
  return str_results;
}

char* get_next_line(const char **data) {
  if(**data == '\0') return NULL;

  const char *line_feed = NULL;
  char *buf = NULL;
  size_t line_size = 0;

  // strchrnul() emulation
  line_feed = strchr(*data, '\n');
  if(line_feed == NULL) line_feed = *data + strlen(*data) - 1;

  line_size = line_feed - *data + 1;
  buf = strndup(*data, line_size);
  *data = line_feed + 1;
  return buf;
}

int transpile_data(const char *data, struct dstring *results) {
  struct parser_results res = {0};
  char *line = NULL, *result_line = NULL;

#ifdef OS_SCRIPT_HEADER
  dstring_addString(results, OS_SCRIPT_HEADER, strlen(OS_SCRIPT_HEADER));
#endif
  while((line = get_next_line(&data)) != NULL) {
    if(parse_line(line, &res) != 0) {
      dstring_clearAll(results);
      free(line);
      return -1;
    }

    if(res.cmd == CMDCODE_INVALID) {
      free(line);
      continue;
    }

    result_line = exec_from_results(&res);
    if(result_line == NULL) {
      dstring_clearAll(results);
      free(line);
      return -1;
    }

    dstring_addString(results, result_line, strlen(result_line));
    free(result_line);
    free(line);
  }

  return 0;
}

int transpile_file(struct fs *system, const char *filename) {
  struct dstring results = {0};
  struct fs_item *item = getFileFromFileSystem(system, filename);
  if(item == NULL) return 0; // Non-existing files shouldn't count as errors

  char *data = malloc(item->dsize + 1);
  if(data == NULL) return -1;
  memcpy(data, item->data, item->dsize);
  data[item->dsize] = '\0';

  if(transpile_data(data, &results) != 0) {
    free(data);
    return -1;
  }

  removeFileFromFileSystem(system, filename);
  addFileToFileSystem(system, filename, results.buf, results.size);

  dstring_clearAll(&results);
  free(data);
  return 0;
}

int execute(struct fs *system) {
  int transpiled = 0;

  if(transpile_file(system, CLIINSTALLER_INSTALL_FILENAME) == 0) {transpiled += 1;}
  else                                                           {return -1;}

  if(transpile_file(system, CLIINSTALLER_UNINSTALL_FILENAME) == 0) {transpiled += 1;}
  else                                                             {return -1;}

  if(transpiled > 0) {
    removeFileFromFileSystem(system, EASYPACK_AUTORUN_NAME);
    addFileToFileSystem(system, EASYPACK_AUTORUN_NAME, OS_STARTUP_SCRIPT, strlen(OS_STARTUP_SCRIPT));
  }

  return 0;
}
