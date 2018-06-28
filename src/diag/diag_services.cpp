#include "diag_services.h"

#include "system.h"
#include "diag.h"

/* for atoi */
#include <stdlib.h> 


/**
 * Diagnostic service implementation to set the sensor cycle timer.
 */
static diag_err_t diag_set_timer(char const * key, char const * val)
{
    int time = atoi(val);
            
    if((time >= 50) && (time <= (5 * 60 * 100)))
    {
        sys_config.sens_cycle_time = time;
        
        return diag_err_ok;
    }
    else
    {
        return diag_err_value;
    }
}


/**
 * Diagnostic service implementation to trigger system reset.
 * 
 * Reset is not triggered in this function, but later by system component. 
 */
static diag_err_t diag_do_reset(char const * key, char const * val)
{
    system_request_reset();

    return diag_err_ok;
}


/* Table mapping service keys to service implementations. */
diag_tbl_t diag_service_tbl[] =
{
    { "time", diag_set_timer },
    { "reset", diag_do_reset },
    { {'\0'}, NULL },
};
