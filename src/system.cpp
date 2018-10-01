#include "system.h"

#include <Arduino.h>
#include <stdint.h>

#include "cfg/cfg.h"
#include "io/sensor.h"
#include "utils/det.h"
#include "utils/sm.h"
#include "net/mqtt.h"
#include "net/wifi.h"

#define SYS_STATE_INIT 0
#define SYS_STATE_IDLE 1
#define SYS_STATE_PUBLISH 2
#define SYS_STATE_RESET 3

#define SYS_PUB_INTERVAL 1000


typedef struct
{
    bool reset_request;
    uint16_t pub_timer;
} sys_data_t;


/**
 * Internal data of the system component.
 */
static sys_data_t sys_data =
{
    .reset_request = false,
    .pub_timer = SYS_PUB_INTERVAL,
};


/**
 * Statemachine handler for the INIT state.
 *
 * Initialize the system and then proceed to IDLE state.
 */
static sm_state_t sys_do_init(void)
{    
    return SYS_STATE_IDLE;
}


/**
 * Statemachine handler for the IDLE state.
 */
static sm_state_t sys_do_idle(void)
{
    /* Reset request has highest priority */
    if(sys_data.reset_request == true)
    {
        return SYS_STATE_RESET;
    }
    else
    {
        return SYS_STATE_IDLE;
    }

    if(sys_data.pub_timer > 0)
    {
        --sys_data.pub_timer;
    }
    else
    {
        sys_data.pub_timer = SYS_PUB_INTERVAL;

        return SYS_STATE_PUBLISH;
    }
    
}


static sm_state_t sys_do_publish(void)
{
    char buf[32];

    /* publish rssi level */
    vfct_fmt(buf, 32, &wifi_vfct_rssi, NULL);
    mqtt_publish("rssi", buf);

    
    return SYS_STATE_IDLE;
}


static sm_state_t sys_do_reset(void)
{
    ESP.reset();

    /* This should never be reached. If still reset is not possible, we will go
     * through reset. */
    DET_ASSERT(0);
    
    return SYS_STATE_INIT;
}


static sm_tbl_entry_t sys_sm_tbl[] =
{
    SM_TBL_ENTRY(sys_do_init, NULL, NULL),
    SM_TBL_ENTRY(sys_do_idle, NULL, NULL),
    SM_TBL_ENTRY(sys_do_publish, NULL, NULL),
    SM_TBL_ENTRY(sys_do_reset, NULL, NULL),
};


static sm_cfg_t sys_sm_cfg = SM_DEF_CFG(SYS_STATE_INIT, sys_sm_tbl);

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
