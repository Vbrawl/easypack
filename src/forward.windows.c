#include "forward.h"
#include <Windows.h>

int setEnvironmentVariable(const char* name, const char* value) {
  if (SetEnvironmentVariable(name, value) == 0) {
    DWORD error = GetLastError();
    printf("setEnvironment(): %lu\n", error);
    return -1;
  }
  return 0;
}

int markAsExecutable(const char* name) {
  return 0; // Nothing to do!
}

void execAndReplace(const char* name, char* const* argv) {
  size_t i = 0;

  STARTUPINFO si = {0}; si.cb = sizeof(si);
  PROCESS_INFORMATION pi = {0};

  if (CreateProcess(
      name,
      GetCommandLine(),
      NULL,
      NULL,
      FALSE,
      0,
      NULL,
      NULL,
      &si,
      &pi) == 0) {
    printf("execAndReplace(): %lu\n", GetLastError());
    return -1;
  }

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
  exit(0);
}
