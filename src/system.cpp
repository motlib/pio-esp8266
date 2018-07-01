#include "system.h"

#include <Arduino.h>
#include "sensor.h"
#include <stdint.h>

#include "cfg.h"


typedef enum
{
    STATE_INIT,
    STATE_WIFI_CONNECT,
    STATE_IDLE,
    STATE_READ,
    STATE_RESET 
} sys_state_t;


typedef struct
{
    sys_state_t state;
    uint16_t sens_timer;
    bool reset_request;
} sys_data_t;



/**
 * Internal data of the system component.
 */
static sys_data_t sys_data =
{
    .state = STATE_INIT,
    .sens_timer = 0,
    .reset_request = false,
};


/**
 * Statemachine handler for the INIT state.
 *
 * Initialize the system and then proceed to IDLE state.
 */
static sys_state_t system_do_init(void)
{
    /* currently nothing to do. Proceed to IDLE. */

    Serial.println(F("BME280"));
    
    return STATE_IDLE;
}


static sys_state_t system_do_wifi_connect(void)
{

    return STATE_IDLE;
}


/**
 * Statemachine handler for the IDLE state.
 * 
 * Do the timekeeping for changing to READ state regularly. 
 */
static sys_state_t system_do_idle()
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
static sys_state_t system_do_read()
{
    sensor_print_values();
    
    return STATE_IDLE;
}


static sys_state_t system_do_reset()
{
    ESP.reset();

    /* This should never be reached. If still reset is not possible, we will go
     * through reset. */
    return STATE_INIT;
}


/**
 * System component main function.
 *
 * This implements the overall system statemachine. 
 */
void system_main(void)
{
    switch(sys_data.state)
    {
    case STATE_INIT:
        sys_data.state = system_do_init();
        break;
        
    case STATE_IDLE:
        sys_data.state = system_do_idle();
        break;
        
    case STATE_READ:
        sys_data.state = system_do_read();
        break;

    case STATE_RESET:
        sys_data.state = system_do_reset();
        break;
        
    default:
        sys_data.state = STATE_INIT;
        break;
    }
}

void system_request_reset(void)
{
    sys_data.reset_request = true;
}
