#include "diag_services.h"

#include "system.h"
#include "diag.h"
#include "uptime.h"
#include "cfg.h"

/* for atoi */
#include <stdlib.h>
/* snprintf */
#include <stdio.h>


/**
 * Diagnostic service implementation to handle the sensor cycle timer.
 */
static diag_err_t diag_sensor_timer(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_write)
    {
        int time = atoi(val);

        if((time >= 50) && (time <= (5 * 60 * 100)))
        {
            cfg.sens_cycle_time = time;

            return diag_err_ok;
        }
        else
        {
            return diag_err_value;
        }
    }
    else if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%i", cfg.sens_cycle_time);

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }

}


/**
 * Diagnostic service implementation to read the uptime counter.
 */
static diag_err_t diag_uptime(char const * const key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%i", uptime_get_seconds());

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


/**
 * Diagnostic service implementation to trigger system reset.
 *
 * Reset is not triggered in this function, but later by system component.
 */
static diag_err_t diag_do_reset(char const * const key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_write)
    {
        system_request_reset();
        return diag_err_ok;
    }

    return diag_err_mode_unsupported;
}


/**
 * Diagnostic service implementation to handle the sensor cycle timer.
 */
static diag_err_t diag_load_cfg(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_write)
    {
        /* TODO: report error. */
        cfg_load();

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }

}

/**
 * Diagnostic service implementation to handle the sensor cycle timer.
 */
static diag_err_t diag_save_cfg(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_write)
    {
        cfg_save();

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
    
}


/* Table mapping service keys to service implementations. */
diag_tbl_t diag_service_tbl[] =
{
    { "time", diag_sensor_timer },
    { "reset", diag_do_reset },
    { "uptime", diag_uptime },
    { "cfgload", diag_load_cfg },
    { "cfgsave", diag_save_cfg },
    { {'\0'}, NULL },
};
