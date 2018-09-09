/**
 * @file
 * 
 * This module contains a statemachine to handle the wifi connection
 * process. The wifi state (ONLINE, OFFLINE) can be requested. If the wifi
 * disconnects, the module will try to reconnect. 
 */

#include "cfg/cfg.h"
#include "diag/diag.h"
#include "diag/diag_services.h"
#include "io/led.h"
#include "utils/det.h"
#include "utils/sm.h"
#include "wifi.h"
#include "wifi_cfg.h"

#include <stdint.h>
#include <ESP8266WiFi.h>


#define WIFI_LED_OFFLINE_BLINK_COUNT   0
#define WIFI_LED_GO_ONLINE_BLINK_COUNT 2
#define WIFI_LED_ONLINE_BLINK_COUNT    1


typedef struct
{
    /** Request to go online. */
    uint8_t request;

    /* The current wifi connection state. */
    uint8_t wifi_state;

    /** Wifi connect timeout. */
    uint16_t timeout;

    uint32_t connect_counter;
} wifi_data_t;


/**
 * Internal data for wifi handling.
 */
static wifi_data_t wifi_data =
{
    .request = WIFI_OFFLINE,
    .wifi_state = WIFI_OFFLINE,
    .timeout = 0,
    .connect_counter = 0,
};


/**
 * Handle when entering the offline state. 
 */
static void wifi_entry_offline(void)
{
    /* no flash to indicate offline mode. */
    led_set_flash_count(&led_stat, WIFI_LED_OFFLINE_BLINK_COUNT);
}


/**
 * Statemachine handler for WIFI_OFFLINE state.
 */    
static sm_state_t wifi_do_offline(void)
{
    /* Check if we shall go online and if the wifi name and password are set. */
    if((wifi_data.request == WIFI_ONLINE)
       && (cfg_wifi.wifi_name[0] != '\0')
       && (cfg_wifi.wifi_password[0] != '\0'))
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
    /* two flashes to indicate connecting */
    led_set_flash_count(&led_stat, WIFI_LED_GO_ONLINE_BLINK_COUNT);
    
    /* Rewind the timeout counter. */
    wifi_data.timeout = WIFI_CONNECT_TIMEOUT;

    WiFi.begin(cfg_wifi.wifi_name, cfg_wifi.wifi_password);

    if(wifi_data.connect_counter < 0xffffffffu)
    {
        ++wifi_data.connect_counter;
    }
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
    /* one flash to indicate connection */
    led_set_flash_count(&led_stat, WIFI_LED_ONLINE_BLINK_COUNT);
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
    SM_TBL_ENTRY(wifi_do_offline, wifi_entry_offline, NULL),
    SM_TBL_ENTRY(wifi_do_go_online, wifi_entry_go_online, NULL),
    SM_TBL_ENTRY(wifi_do_online, wifi_entry_online, NULL),
};


/* Statemachine configuration */
static sm_cfg_t wifi_sm_cfg = SM_DEF_CFG(WIFI_OFFLINE, wifi_sm_tbl);


/* Statemachine run-time data */
static sm_data_t wifi_sm_data = SM_DEF_DATA();


/**
 * Initialize the wifi sub-system.
 */
void wifi_init(void)
{
    WiFi.setAutoConnect(false);

    /* Reconnection will be handled by this statemachine. */
    WiFi.setAutoReconnect(false);

    /* If we shall enable the wifi and name and password are set, we go
     * online. */
    if(cfg_wifi.wifi_power_on_state != 0)
    {
        wifi_request_state(WIFI_ONLINE);
    }
    else
    {
        wifi_request_state(WIFI_OFFLINE);
    }
}


/**
 * Main function for the wifi handler.
 */
void wifi_main(void)
{
    sm_step(&wifi_sm_cfg, &wifi_sm_data);
}


/**
 * Return the current state of the wifi statemachine.
 * 
 * @returns One of the WIFI_OFFLINE, WIFI_GO_ONLINE, WIFI_ONLINE constants.
 */
uint8_t wifi_get_state(void)
{
    return wifi_sm_data.state;
}


/**
 * Request the state of the wifi to be online or offline.
 *
 * @param state 0 to go offline, any other value to go online.
 */
void wifi_request_state(uint8_t state)
{
    if(state)
    {
        wifi_data.request = WIFI_ONLINE;
    }
    else
    {
        wifi_data.request = WIFI_OFFLINE;
    }
}


/**
 * Diagnostic service to print current connection data for wifi.
 */
diag_err_t wifi_diag_status(char const * key, char * const val, diag_mode_t mode, void * const extra_data)
{
    if(mode == diag_mode_read)
    {
        IPAddress addr;
        
        snprintf(val, DIAG_VAL_BUF_LEN, "hostname=%s", WiFi.hostname().c_str());
        diag_print_data(val);

        addr = WiFi.localIP();
        snprintf(val, DIAG_VAL_BUF_LEN, "local-ip=%i.%i.%i.%i",
                 addr[0], addr[1], addr[2], addr[3]);
        diag_print_data(val);

        addr = WiFi.subnetMask();
        snprintf(val, DIAG_VAL_BUF_LEN, "mask=%i.%i.%i.%i",
                 addr[0], addr[1], addr[2], addr[3]);
        diag_print_data(val);

        addr = WiFi.gatewayIP();
        snprintf(val, DIAG_VAL_BUF_LEN, "gateway=%i.%i.%i.%i",
                 addr[0], addr[1], addr[2], addr[3]);
        diag_print_data(val);
        
        addr = WiFi.dnsIP();
        snprintf(val, DIAG_VAL_BUF_LEN, "dns=%i.%i.%i.%i",
                 addr[0], addr[1], addr[2], addr[3]);
        diag_print_data(val);

        snprintf(val, DIAG_VAL_BUF_LEN, "rssi=%i", WiFi.RSSI());
        diag_print_data(val);

        snprintf(val, DIAG_VAL_BUF_LEN, "connect-count=%u", wifi_data.connect_counter);
        diag_print_data(val);
        
        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}
