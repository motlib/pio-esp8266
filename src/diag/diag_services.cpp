#include "diag_services.h"

#include "system.h"
#include "diag.h"
#include "uptime.h"
#include "cfg/cfg.h"
#include "net/wifi.h"
#include "version.h"
#include "sensor.h"
#include "version.h"

/* for atoi */
#include <stdlib.h>
/* snprintf */
#include <stdio.h>
/* Serial */
#include <Arduino.h>
#include <ESP8266httpUpdate.h>

#include "utils/vfct.h"



/**
 * Generic service implementation to handle vfct values. 
 */
static diag_err_t diag_vfct_handler(char const * const key, char * const val, diag_mode_t mode, void * const extra_data)
{
    vfct_t * const vfct_ptr = (vfct_t * const)extra_data;

    /* We only check get_float member. As all pointers are stored as union, it
     * doesn't matter, which pointer to check for NULL. Same below for write
     * mode. */
    if((mode == diag_mode_read) && (vfct_ptr->get_fct.get_float != NULL))
    {
        (void)vfct_fmt(val, DIAG_VAL_BUF_LEN, vfct_ptr, NULL);
        diag_print_data(val);

        return diag_err_ok;
    }
    else if((mode == diag_mode_write) && (vfct_ptr->set_fct.set_float != NULL))
    {
        vfct_err_t res;
        res = vfct_parse(vfct_ptr, val);

        if(res == VFCT_ERR_OK)
        {
            return diag_err_ok;
        }
        else
        {
            return diag_err_value;
        }
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


/**
 * Diagnostic service implementation to trigger system reset.
 *
 * Reset is not triggered in this function, but later by system component.
 */
static diag_err_t diag_do_reset(char const * const key, char * const val, diag_mode_t mode, void * const extra_data)
{
    if(mode == diag_mode_write)
    {
        system_request_reset();
        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


/**
 * Diagnostic service implementation to load the system configuration from
 * eeprom.
 */
static diag_err_t diag_load_cfg(char const * key, char * const val, diag_mode_t mode, void * const extra_data)
{
    if(mode == diag_mode_write)
    {
        /* TODO: check / report error. */
        cfg_load();

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


/**
 * Diagnostic service implementation to save the system configuration to eeprom.
 */
static diag_err_t diag_save_cfg(char const * key, char * const val, diag_mode_t mode, void * const extra_data)
{
    if(mode == diag_mode_write)
    {
        cfg_save();

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


/**
 * Diagnostic service implementation to handle the sensor cycle timer.
 */
static diag_err_t diag_wifi_state(char const * key, char * const val, diag_mode_t mode, void * const extra_data)
{
    if(mode == diag_mode_write)
    {
        int state = atoi(val);

        if((state == WIFI_OFFLINE) || (state == WIFI_ONLINE))
        {
            wifi_request_state(state);

            return diag_err_ok;
        }
        else
        {
            return diag_err_value;
        }
    }
    else if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%i", wifi_get_state());
        diag_print_data(val);

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


/**
 * Diagnostic service implementation to access the wifi power-on state setting.
 */
static diag_err_t diag_wifi_pon_connect(char const * key, char * const val, diag_mode_t mode, void * const extra_data)
{
    if(mode == diag_mode_write)
    {
        int state = atoi(val);

        if((state == 0) || (state == 1))
        {
            cfg_wifi.wifi_power_on_state = state;
            return diag_err_ok;
        }
        else
        {
            return diag_err_value;
        }
    }
    else if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%i", cfg_wifi.wifi_power_on_state);
        diag_print_data(val);

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


/**
 * Read the firmware version.
 */
static diag_err_t diag_fw_version(char const * key, char * const val, diag_mode_t mode, void * const extra_data)
{
    if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "fw_version=%s", version_data.fw_version);
        diag_print_data(val);

        snprintf(val, DIAG_VAL_BUF_LEN, "build_date=%s", version_data.build_date);
        diag_print_data(val);

        snprintf(val, DIAG_VAL_BUF_LEN, "git_hash=%s", version_data.git_hash);
        diag_print_data(val);

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


/**
 * Trigger the OTA update process.
 */
static diag_err_t diag_ota(char const * key, char * const val, diag_mode_t mode, void * const extra_data)
{
    if(mode == diag_mode_write)
    {
        HTTPUpdateResult res;

        res = ESPhttpUpdate.update(cfg.ota_host, cfg.ota_port, cfg.ota_path);

        /* updater only returns, if nothing to do or error. On success, the MCU
         * does a reset. */

        snprintf(val, DIAG_VAL_BUF_LEN, "res=%i", res);
        diag_print_data(val);

        snprintf(val, DIAG_VAL_BUF_LEN, "err=%i", ESPhttpUpdate.getLastError());
        diag_print_data(val);

        snprintf(val, DIAG_VAL_BUF_LEN, "str=%s", ESPhttpUpdate.getLastErrorString().c_str());

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }

}




/**
 * Read or set the OTA URL path.
 */
static diag_err_t diag_mqtt_port(char const * key, char * const val, diag_mode_t mode, void * const extra_data)
{
    if(mode == diag_mode_write)
    {
        int port = atoi(val);

        cfg_mqtt.port = port;

        return diag_err_ok;
    }
    else if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%u", cfg_mqtt.port);
        diag_print_data(val);

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


static diag_err_t diag_help(char const * key, char * const val, diag_mode_t mode, void * const extra_data);


/* Table mapping service keys to service implementations. */
diag_tbl_t const diag_service_tbl[] =
{
    { "cfg-load", diag_load_cfg, NULL },
    { "cfg-save", diag_save_cfg, NULL },
    { "help", diag_help, NULL },
    { "mqtt-broker", diag_vfct_handler, (void * const)&cfg_vfct_mqtt_broker },
    { "mqtt-pass", diag_vfct_handler, (void * const)&cfg_vfct_mqtt_password },
    { "mqtt-port", diag_mqtt_port, NULL },
    { "mqtt-user", diag_vfct_handler, (void * const)&cfg_vfct_mqtt_user },
    { "node-name", diag_vfct_handler, (void * const)&cfg_vfct_node_name },
    { "ota", diag_ota, NULL },
    { "ota-host", diag_vfct_handler, (void * const)&cfg_vfct_ota_host },
    { "ota-path", diag_vfct_handler, (void * const)&cfg_vfct_ota_path },
    { "ota-port", diag_vfct_handler, (void * const)&cfg_vfct_ota_port },
    { "reset", diag_do_reset, NULL },
    { "sens-read", sensor_diag_info, NULL },
    { "sens-time", diag_vfct_handler, (void * const)&sensor_vfct_timer },
    { "ts-channel", diag_vfct_handler, (void * const)&cfg_vfct_ts_channel },
    { "ts-chkey", diag_vfct_handler, (void * const)&cfg_vfct_ts_channel_key },
    { "uptime", diag_vfct_handler, (void * const)&uptime_seconds_vfct },
    { "vers", diag_fw_version, NULL },
    { "wifi", diag_wifi_state, NULL },
    { "wifi-name", diag_vfct_handler, (void * const)&cfg_vfct_wifi_name },
    { "wifi-pass", diag_vfct_handler, (void * const)&cfg_vfct_wifi_password },
    { "wifi-pon", diag_wifi_pon_connect, NULL },
    { "wifi-stat", wifi_diag_status, NULL },

    { {'\0'}, NULL, NULL },
};

static diag_err_t diag_help(char const * key, char * const val, diag_mode_t mode, void * const extra_data)
{
    if(mode == diag_mode_read)
    {
        for(diag_tbl_t const * entry = &(diag_service_tbl[0]);
            entry->svc_fct != NULL;
            ++entry)
        {
            diag_print_data(entry->key);
        }

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}
