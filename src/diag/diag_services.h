#pragma once

#include "diag.h"

typedef struct
{
    char key[8];

    /**
     * Diagnostic service function pointer.
     * @param key Service key (name)
     * @param value Value to set
     */
    diag_err_t (*svc_fct)(char const * key, char const * value);
} diag_tbl_t;

extern diag_tbl_t diag_service_tbl[];
