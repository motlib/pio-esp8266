
#include "wifi.h"
#include "utils/sm.h"
#include "cfg/cfg.h"

#include <stdint.h>
#include <ESP8266WiFi.h>

#define WIFI_CONNECT_TIMEOUT 1000




typedef struct
{
    /** Request to go online. */
    uint8_t request;

    /* The current wifi connection state. */
    uint8_t wifi_state;

    /** Wifi connect timeout. */
    uint16_t timeout;
} wifi_data_t;


static wifi_data_t wifi_data = {
    .request = WIFI_OFFLINE,
    .wifi_state = WIFI_OFFLINE,
    .timeout = 0,
};


    
static sm_state_t wifi_do_offline(void)
{
    if(wifi_data.request == 1)
    {
        return WIFI_GO_ONLINE;
    }
    else
    {
        return WIFI_OFFLINE;
    }
}

static void wifi_entry_go_online(void)
{
    /* Rewind the timeout counter. */
    wifi_data.timeout = WIFI_CONNECT_TIMEOUT;

    /* Reconnection will be handled by this statemachine. */
    WiFi.setAutoReconnect(false);
    
    WiFi.begin(cfg.wifi, cfg.password);
}

static sm_state_t wifi_do_go_online(void)
{
    if(wifi_data.timeout == 0)
    {
        return WIFI_OFFLINE;
    }
    else
    {
        --wifi_data.timeout;

        if(WiFi.status() == WL_CONNECTED)
        {
            return WIFI_ONLINE;
        }
        else
        {
            return WIFI_GO_ONLINE;
        }
    }
}

static sm_state_t wifi_do_online(void)
{
    if(!wifi_data.request)
    {
        WiFi.disconnect();
        
        return WIFI_OFFLINE;
    }
    else
    {

        if(WiFi.status() != WL_CONNECTED)
        {
            return WIFI_GO_ONLINE;
        }
        else
        {
            return WIFI_ONLINE;
        }
    }
}

static sm_tbl_entry_t wifi_sm_tbl[] = 
{
    SM_TBL_ENTRY(wifi_do_offline, NULL, NULL),
    SM_TBL_ENTRY(wifi_do_go_online, wifi_entry_go_online, NULL),
    SM_TBL_ENTRY(wifi_do_online, NULL, NULL),
};

static sm_cfg_t wifi_sm_cfg = SM_DEF_CFG(WIFI_OFFLINE, wifi_sm_tbl);

static sm_data_t wifi_sm_data = SM_DEF_DATA();

void wifi_main(void)
{
    sm_step(&wifi_sm_cfg, &wifi_sm_data);
}


uint8_t wifi_get_state(void)
{
    return wifi_data.wifi_state;
}

void wifi_request_state(uint8_t state)
{
    wifi_data.request = state;
}
