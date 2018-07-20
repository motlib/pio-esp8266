#include "version.h"
#include "version_cfg.h"

#include "version_info.h"


version_data_t version_data =
{
    /* the firmware version */
    .fw_version = VERSION_FW,

    /* the build date. */
    .build_date = VERSION_BUILD_DATE,

    .git_hash = VERSION_GIT_HASH
};
