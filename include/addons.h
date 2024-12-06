#ifndef EASYPACK_ADDONS_H
#define EASYPACK_ADDONS_H

#ifndef ENV_EASYPACK_ADDONS
#define ENV_EASYPACK_ADDONS "EASYPACK_ADDONS"
#endif

#ifndef EASYPACK_ADDONS_SEPARATOR
#define EASYPACK_ADDONS_SEPARATOR ','
#endif

#include "embedded_filesystem.h"

int executeAddons(struct fs *system);


#endif
