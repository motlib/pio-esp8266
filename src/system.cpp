#include "system.h"

#include "sensor.h"
#include <stdint.h>


typedef enum
{
    STATE_INIT = 0,
    STATE_IDLE = 1,
    STATE_READ = 2,
} sys_state_t;


typedef struct
{
    sys_state_t state;
    uint16_t sens_timer;
} sys_data_t;


/**
 * Configuration of the system component.
 */
sys_config_t sys_config =
{
    .sens_cycle_time = 100,
};


/**
 * Internal data of the system component.
 */
static sys_data_t sys_data =
{
    .state = STATE_INIT,
    .sens_timer = 0,
};



/**
 * Statemachine handler for the INIT state.
 *
 * Initialize the system and then proceed to IDLE state.
 */
static sys_state_t system_do_init()
{
    /* currently nothing to do. Proceed to IDLE. */
    
    return STATE_IDLE;
}


/**
 * Statemachine handler for the IDLE state.
 * 
 * Do the timekeeping for changing to READ state regularly. 
 */
static sys_state_t system_do_idle()
{
    if(sys_data.sens_timer != 0)
    {
        sys_data.sens_timer -= 1;

        return STATE_IDLE;
    }
    else
    {
        sys_data.sens_timer = sys_config.sens_cycle_time;
        
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


/**
 * System component main function.
 *
 * This implements the overall system statemachine. 
 */
void system_main()
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
        
    default:
        sys_data.state = STATE_INIT;
        break;
    }
}

// TODO;
void system_set_sensor_cycle_time();
