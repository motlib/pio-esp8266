#include "system.h"

#include <Arduino.h>
#include <stdint.h>

#include "sensor.h"
#include "utils/sm.h"
#include "cfg.h"


#define STATE_INIT 0
#define STATE_WIFI_CONNECT 1
#define STATE_IDLE 2
#define STATE_READ 3
#define STATE_RESET 4


typedef struct
{
    uint16_t sens_timer;
    bool reset_request;
} sys_data_t;



/**
 * Internal data of the system component.
 */
static sys_data_t sys_data =
{
    .sens_timer = 0,
    .reset_request = false,
};


/**
 * Statemachine handler for the INIT state.
 *
 * Initialize the system and then proceed to IDLE state.
 */
static sm_state_t sys_do_init(void)
{
    /* currently nothing to do. Proceed to IDLE. */

    Serial.println(F("BME280"));
    
    return STATE_IDLE;
}


static sm_state_t sys_do_wifi_connect(void)
{

    return STATE_IDLE;
}


/**
 * Statemachine handler for the IDLE state.
 * 
 * Do the timekeeping for changing to READ state regularly. 
 */
static sm_state_t sys_do_idle()
{
    /* Reset request has highest priority */
    if(sys_data.reset_request == true)
    {
        return STATE_RESET;
    }
    
    if(sys_data.sens_timer != 0)
    {
        sys_data.sens_timer -= 1;

        return STATE_IDLE;
    }
    else
    {
        sys_data.sens_timer = cfg.sens_cycle_time;
        
        return STATE_READ;
    }
}


/**
 * Statemachine handler for the READ state.
 *
 * Sample the sensor, print the values and return to IDLE state.
 */
static sm_state_t sys_do_read()
{
    sensor_print_values();
    
    return STATE_IDLE;
}


static sm_state_t sys_do_reset()
{
    ESP.reset();

    /* This should never be reached. If still reset is not possible, we will go
     * through reset. */
    return STATE_INIT;
}


static sm_tbl_entry_t sys_sm_tbl[] = {
    SM_TBL_ENTRY(sys_do_init, NULL, NULL),
    SM_TBL_ENTRY(sys_do_idle, NULL, NULL),
    SM_TBL_ENTRY(sys_do_read, NULL, NULL),
    SM_TBL_ENTRY(sys_do_reset, NULL, NULL),
};


static sm_cfg_t sys_sm_cfg = SM_DEF_CFG(STATE_INIT, sys_sm_tbl);

static sm_data_t sys_sm_data = SM_DEF_DATA();



/**
 * System component main function.
 *
 * This implements the overall system statemachine. 
 */
void system_main(void)
{
    sm_step(&sys_sm_cfg, &sys_sm_data);    
}
    
    
void system_request_reset(void)
{
    sys_data.reset_request = true;
}
