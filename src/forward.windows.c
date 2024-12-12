#include "forward.h"
#include <Windows.h>
#include <stdio.h>

API int setEnvironmentVariable(const char* name, const char* value) {
  if (SetEnvironmentVariable(name, value) == 0) {
    DWORD error = GetLastError();
    printf("setEnvironment(): %lu\n", error);
    return -1;
  }
  return 0;
}

API int markAsExecutable(const char* name) {
  return 0; // Nothing to do!
}

API void execAndReplace(const char* name, char* const* argv) {
  // TODO: We need to write a proper parser for generating command lines
  size_t argv0_size = strlen(argv[0]) + 2; // NOTE: argv is always quoted when in cmdline
  char* cmdline = GetCommandLine();
  size_t new_cmdline_size = snprintf(NULL, 0, "%s %s", name, cmdline + argv0_size);
  char* new_cmdline = malloc(new_cmdline_size + 1);
  snprintf(new_cmdline, new_cmdline_size + 1, "%s %s", name, cmdline + argv0_size);
  DWORD exitCode = 0;

  STARTUPINFO si = {0}; si.cb = sizeof(si);
  PROCESS_INFORMATION pi = {0};

  if (CreateProcess(
      name,
      new_cmdline,
      NULL,
      NULL,
      FALSE,
      0,
      NULL,
      NULL,
      &si,
      &pi) == 0) {
    printf("execAndReplace(): %lu\n", GetLastError());
    free(new_cmdline);
    return;
  }
  free(new_cmdline);

  // wait for process to complete
  WaitForSingleObject(pi.hProcess, INFINITE);
  GetExitCodeProcess(pi.hProcess, &exitCode);
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  exit(exitCode);
}
