#include "version.h"
#include "version_cfg.h"

#ifndef BUILD_DATE
#warning BUILD_DATE define must be available!
#endif

/* workaround to stringify the build date. See
* https://gcc.gnu.org/onlinedocs/gcc-3.1/cpp/Stringification.html */
#define XSTR(X) STR(X)
#define STR(X) #X

version_data_t version_data =
{
    /* the firmware version */
    .fw_version = VERSION_FW,

    /* the build date. */
    .build_date = XSTR(BUILD_DATE),
};
