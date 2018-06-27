#include "system.h"

#include "sensor.h"


typedef enum {
    STATE_INIT = 0,
    STATE_IDLE = 1,
    STATE_READ = 2,
} state_t;


sys_config_t sys_config =
{
    .sens_cycle_time = 100;
};



// TODO;
void system_set_sensor_cycle_time();

    
void system_main()
{
    static state_t state = STATE_INIT;
    static int timeout = 0;
    
    switch(state)
    {
    case STATE_INIT:
        state = STATE_IDLE;
        break;
        
    case STATE_IDLE:
        if(timeout != 0)
        {
            timeout -= 1;
        }
        else
        {
            timeout = sys_config.sens_cycle_time;
            state = STATE_READ;
        }
        
        break;
        
    case STATE_READ:
        sensor_print_values();
        state = STATE_IDLE;
        break;
        
    default:
        state = STATE_INIT;
        break;
                    
    }
}
