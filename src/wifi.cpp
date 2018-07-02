
#include "wifi.h"
#include "statemachine.h"

#include <stdint.h>

#define WIFI_CONNECT_TIMEOUT 1000

/* States */
#define WIFI_OFFLINE 0
#define WIFI_GO_ONLINE 1
#define WIFI_ONLINE 2


typedef struct
{
    /** Request to go online. */
    uint8_t request;

    /** Wifi connect timeout. */
    uint16_t timeout;
} wifi_data_t;


static wifi_data_t wifi_data = {
    .request = 0,
    
    .timeout = 0,
};


void wifi_init(void)
{
    
}
    
static sm_state_t wifi_do_offline(void)
{
    if(wifi_data.request == 1)
    {
        wifi_data.timeout = WIFI_CONNECT_TIMEOUT;
        return WIFI_GO_ONLINE;
    }
    else
    {
        return WIFI_OFFLINE;
    }
}

static sm_state_t wifi_do_go_online(void)
{
    //WiFi.begin(ssid, password);
    //
    //while (WiFi.status() != WL_CONNECTED) {
    //
    //    delay(1000);
    //    Serial.println("Connecting..");
    //    
    //}

    return WIFI_ONLINE;
}

static sm_state_t wifi_do_online(void)
{
    return WIFI_ONLINE;
}

sm_tbl_entry_t wifi_sm_tbl[] = 
{
    { .do_fct = wifi_do_offline, .entry_fct = NULL, .exit_fct = NULL },
    { .do_fct = wifi_do_go_online, .entry_fct = NULL, .exit_fct = NULL },
    { .do_fct = wifi_do_online, .entry_fct = NULL, .exit_fct = NULL },
    /* table end marker */
    { .do_fct = NULL }, 
};

sm_cfg_t wifi_sm_cfg =
{
    .init_state = WIFI_OFFLINE,
    .tbl = wifi_sm_tbl,
};

sm_data_t wifi_sm_data = {
    .state = WIFI_OFFLINE,
};

void wifi_main(void)
{
    sm_step(&wifi_sm_cfg, &wifi_sm_data);
}
