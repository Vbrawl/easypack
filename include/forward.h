#ifndef EASYPACK_FORWARD_H
#define EASYPACK_FORWARD_H

#ifndef ENVOUT_EASYPACK_NAME
#define ENVOUT_EASYPACK_NAME "EASYPACK_NAME"
#endif

#ifndef ENVOUT_EASYPACK_CWD
#define ENVOUT_EASYPACK_CWD "EASYPACK_CWD"
#endif

#ifndef EASYPACK_AUTORUN_NAME
#define EASYPACK_AUTORUN_NAME "autorun.easypack"
#endif


int prepareEnvironment();
void executeAutoRun(const char *autorun_name, char *const *argv);

#endif
