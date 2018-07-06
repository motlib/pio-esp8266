/**
 * @file
 * 
 * This module contains a statemachine to handle the wifi connection
 * process. The wifi state (ONLINE, OFFLINE) can be requested. If the wifi
 * disconnects, the module will try to reconnect. 
 */

#include "wifi.h"
#include "wifi_cfg.h"
#include "utils/sm.h"
#include "cfg/cfg.h"
#include "utils/det.h"

#include "diag/diag_services.h"

#include <stdint.h>
#include <ESP8266WiFi.h>



typedef struct
{
    /** Request to go online. */
    uint8_t request;

    /* The current wifi connection state. */
    uint8_t wifi_state;

    /** Wifi connect timeout. */
    uint16_t timeout;
} wifi_data_t;


/**
 * Internal data for wifi handling.
 */
static wifi_data_t wifi_data =
{
    .request = WIFI_OFFLINE,
    .wifi_state = WIFI_OFFLINE,
    .timeout = 0,
    
};


/**
 * Statemachine handler for WIFI_OFFLINE state.
 */    
static sm_state_t wifi_do_offline(void)
{
    if(wifi_data.request == WIFI_ONLINE)
    {
        return WIFI_GO_ONLINE;
    }
    else
    {
        return WIFI_OFFLINE;
    }
}


/**
 * Statemachine entry handler for WIFI_GO_ONLINE state.
 */
static void wifi_entry_go_online(void)
{
    /* Rewind the timeout counter. */
    wifi_data.timeout = WIFI_CONNECT_TIMEOUT;

    /* Reconnection will be handled by this statemachine. */
    WiFi.setAutoReconnect(false);
    
    WiFi.begin(cfg.wifi, cfg.password);
    Serial.println(F("i:wifi=connecting"));
}


/**
 * Statemachine handler for WIFI_GO_ONLINE state.
 */
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


/** 
 * Statemachine entry handler for online state. 
 */
static void wifi_entry_online(void)
{
    Serial.println(F("i:wifi=connected"));
}


/**
 * Statemachine handler for online state.
 */
static sm_state_t wifi_do_online(void)
{
    /* If we are asked to go offline, we disconnect and transition to the
     * WIFI_OFFLINE state. */
    if(wifi_data.request == WIFI_OFFLINE)
    {
        WiFi.disconnect();
        
        return WIFI_OFFLINE;
    }
    else if(wifi_data.request == WIFI_ONLINE)
    {
        /* We are asked to be online, but we are not connect, so return to the
         * WIFI_GO_ONLINE state and try again. */
        if(WiFi.status() != WL_CONNECTED)
        {
            return WIFI_GO_ONLINE;
        }
        else
        {
            /* We are online, eveything is fine. */
            return WIFI_ONLINE;
        }
    }
    else
    {
        /* Hm, we should not be here. We are asked to be in an invalid wifi
         * state. In development version, we use the DET. In production, we just
         * stay online.*/
        
        DET_ASSERT(0);

        return WIFI_ONLINE;
    }
}

/* Statemachine table for wifi handling */
static sm_tbl_entry_t wifi_sm_tbl[] = 
{
    SM_TBL_ENTRY(wifi_do_offline, NULL, NULL),
    SM_TBL_ENTRY(wifi_do_go_online, wifi_entry_go_online, NULL),
    SM_TBL_ENTRY(wifi_do_online, wifi_entry_online, NULL),
};

/* Statemachine configuration */
static sm_cfg_t wifi_sm_cfg = SM_DEF_CFG(WIFI_OFFLINE, wifi_sm_tbl);

/* Statemachine run-time data */
static sm_data_t wifi_sm_data = SM_DEF_DATA();


/**
 * Main function for the wifi handler.
 */
void wifi_main(void)
{
    sm_step(&wifi_sm_cfg, &wifi_sm_data);
}


/**
 * Return the current state of the wifi statemachine.
 */
uint8_t wifi_get_state(void)
{
    return wifi_sm_data.state;
}

/**
 * Request the state of the wifi (either WIFI_OFFLINE or WIFI_ONLINE).
 */
void wifi_request_state(uint8_t state)
{
    wifi_data.request = state;
}


/**
 * Diagnostic service to print current connection data for wifi.
 */
diag_err_t diag_wifi_status(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_read)
    {
        Serial.print(F("dda:hostname="));
        Serial.println(WiFi.hostname());
        Serial.print(F("dda:local-ip="));
        Serial.println(WiFi.localIP());
        Serial.print(F("dda:netmask="));
        Serial.println(WiFi.subnetMask());
        Serial.print(F("dda:gateway="));
        Serial.println(WiFi.gatewayIP());
        Serial.print(F("dda:dns="));
        Serial.println(WiFi.dnsIP());
        Serial.print(F("dda:rssi="));
        Serial.println(WiFi.RSSI());

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}
