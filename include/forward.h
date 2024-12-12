#ifndef EASYPACK_FORWARD_H
#define EASYPACK_FORWARD_H

#include "compatibility-layer.h"

#ifndef ENVOUT_EASYPACK_NAME
#define ENVOUT_EASYPACK_NAME "EASYPACK_NAME"
#endif

#ifndef ENVOUT_EASYPACK_CWD
#define ENVOUT_EASYPACK_CWD "EASYPACK_CWD"
#endif

#ifndef EASYPACK_AUTORUN_NAME
#ifdef _WIN32
#define EASYPACK_AUTORUN_NAME "autorun.easypack.bat"
#else
#define EASYPACK_AUTORUN_NAME "autorun.easypack"
#endif
#endif


API int setEnvironmentVariable(const char *name, const char *value);
API int markAsExecutable(const char *name);
API int prepareEnvironment();

/**
 * @warning On windows the argv argument is ignored!
 */
API void executeAutoRun(const char *autorun_name, char *const *argv);
API void execAndReplace(const char *name, char *const *argv);

#endif
