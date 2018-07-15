#include "diag_services.h"

#include "system.h"
#include "diag.h"
#include "uptime.h"
#include "cfg/cfg.h"
#include "net/wifi.h"
#include "version.h"
#include "sensor.h"

/* for atoi */
#include <stdlib.h>
/* snprintf */
#include <stdio.h>
/* Serial */
#include <Arduino.h>
#include <ESP8266httpUpdate.h>



/**
 * Diagnostic service implementation to handle the sensor cycle timer.
 */
static diag_err_t diag_sensor_timer(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_write)
    {
        int time = atoi(val);

        if((time >= 50) && (time <= (5 * 60 * 100)))
        {
            cfg.sens_cycle_time = time;

            return diag_err_ok;
        }
        else
        {
            return diag_err_value;
        }
    }
    else if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%i", cfg.sens_cycle_time);

        diag_print_data(val);

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


/**
 * Diagnostic service implementation to read the uptime counter.
 */
static diag_err_t diag_uptime(char const * const key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%i", uptime_get_seconds());
        diag_print_data(val);

        return diag_err_ok;
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
static diag_err_t diag_do_reset(char const * const key, char * const val, diag_mode_t mode)
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
static diag_err_t diag_load_cfg(char const * key, char * const val, diag_mode_t mode)
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
static diag_err_t diag_save_cfg(char const * key, char * const val, diag_mode_t mode)
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
 * Helper function for reading / wrinting of strings by diagnosis.
 */
static diag_err_t diag_handle_string(char const * key, char * const val, diag_mode_t mode, char* strvar, size_t len)
{
    if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%s", strvar);
        diag_print_data(val);

        return diag_err_ok;
    }
    else if(mode == diag_mode_write)
    {
        strncpy(strvar, val, len);
        /* ensure that string is 0 terminated */
        strvar[len - 1] = '\0';

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }    
}


/**
 * Diagnostic service implementation to read and write wifi name. 
 */
static diag_err_t diag_wifi_name(char const * key, char * const val, diag_mode_t mode)
{
    return diag_handle_string(key, val, mode, cfg_wifi.wifi_name, CFG_WIFI_NAME_LEN);
}


/**
 * Diagnostic service implementation to write the wifi password.
 */
static diag_err_t diag_wifi_password(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_write)
    {
        return diag_handle_string(key, val, mode, cfg_wifi.wifi_password, CFG_WIFI_PWD_LEN);
    }
    else
    {
        return diag_err_mode_unsupported;
    }
} 


/**
 * Diagnostic service implementation to read and write the node name.
 */
static diag_err_t diag_node_name(char const * key, char * const val, diag_mode_t mode)
{
    return diag_handle_string(key, val, mode, cfg_wifi.node_name, CFG_NODE_NAME_LEN);
} 


/**
 * Diagnostic service implementation to handle the sensor cycle timer.
 */
static diag_err_t diag_wifi_state(char const * key, char * const val, diag_mode_t mode)
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
 * Diagnostic service implementation to handle the sensor cycle timer.
 */
static diag_err_t diag_sensor_print(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_write)
    {
        int state = atoi(val);

        if((state == 0) || (state == 1))
        {
            cfg.sens_print = state;

            return diag_err_ok;
        }
        else
        {
            return diag_err_value;
        }
    }
    else if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%i", cfg.sens_print);
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
static diag_err_t diag_wifi_pon_connect(char const * key, char * const val, diag_mode_t mode)
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
 * Read or set the OTA URL host.
 */
static diag_err_t diag_ota_host(char const * key, char * const val, diag_mode_t mode)
{
    return diag_handle_string(key, val, mode, cfg.ota_host, CFG_NODE_NAME_LEN);
}


/**
 * Read or set the OTA URL path.
 */
static diag_err_t diag_ota_port(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_write)
    {
        int port = atoi(val);

        cfg.ota_port = port;
        
        return diag_err_ok;
    }
    else if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%u", cfg.ota_port);
        diag_print_data(val);

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
static diag_err_t diag_ota_path(char const * key, char * const val, diag_mode_t mode)
{
    return diag_handle_string(key, val, mode, cfg.ota_path, CFG_NODE_NAME_LEN);
}


/**
 * Read the firmware version.
 */
static diag_err_t diag_fw_version(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_read)
    {
        strncpy(val, VERSION_FW, DIAG_VAL_BUF_LEN);
        val[DIAG_VAL_BUF_LEN - 1] = '\0';
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
static diag_err_t diag_ota(char const * key, char * const val, diag_mode_t mode)
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


static diag_err_t diag_keys(char const * key, char * const val, diag_mode_t mode);


/* Table mapping service keys to service implementations. */
diag_tbl_t const diag_service_tbl[] =
{
    { "cfgload", diag_load_cfg },
    { "cfgsave", diag_save_cfg },
    { "diagkeys", diag_keys },
    { "fwvers", diag_fw_version },
    { "nodename", diag_node_name },
    { "ota-host", diag_ota_host },
    { "ota-port", diag_ota_port },    
    { "ota-path", diag_ota_path },
    { "ota", diag_ota },
    { "reset", diag_do_reset },
    { "sens", diag_sensor_info },
    { "sprint", diag_sensor_print },
    { "stime", diag_sensor_timer },
    { "uptime", diag_uptime },
    { "wifi", diag_wifi_state },
    { "wifiname", diag_wifi_name },
    { "wifipon", diag_wifi_pon_connect },
    { "wifipwd", diag_wifi_password },
    { "wifistat", diag_wifi_status },
    { {'\0'}, NULL },
};

static diag_err_t diag_keys(char const * key, char * const val, diag_mode_t mode)
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
