
#include "wifi.h"
#include "utils/sm.h"

#include <stdint.h>


#define WIFI_CONNECT_TIMEOUT 1000

/* States */
#define WIFI_OFFLINE 0u
#define WIFI_GO_ONLINE 1u
#define WIFI_ONLINE 2u


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

static sm_tbl_entry_t wifi_sm_tbl[] = 
{
    SM_TBL_ENTRY( wifi_do_offline, NULL, NULL),
    SM_TBL_ENTRY(wifi_do_go_online, NULL, NULL),
    SM_TBL_ENTRY(wifi_do_online, NULL, NULL),
};

static sm_cfg_t wifi_sm_cfg = SM_DEF_CFG(WIFI_OFFLINE, wifi_sm_tbl);

static sm_data_t wifi_sm_data = SM_DEF_DATA();

void wifi_main(void)
{
    sm_step(&wifi_sm_cfg, &wifi_sm_data);
}
