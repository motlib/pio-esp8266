#include "diag_services.h"

#include "system.h"
#include "diag.h"
#include "uptime.h"
#include "cfg/cfg.h"
#include "wifi.h"

/* for atoi */
#include <stdlib.h>
/* snprintf */
#include <stdio.h>
/* Serial */
#include <Arduino.h>

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

    return diag_err_mode_unsupported;
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
    return diag_handle_string(key, val, mode, cfg.wifi_name, CFG_WIFI_NAME_LEN);
}


/**
 * Diagnostic service implementation to write the wifi password.
 */
static diag_err_t diag_wifi_password(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_write)
    {
        return diag_handle_string(key, val, mode, cfg.wifi_password, CFG_WIFI_PWD_LEN);
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
    return diag_handle_string(key, val, mode, cfg.node_name, CFG_NODE_NAME_LEN);
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
            cfg.wifi_power_on_state = state;
            return diag_err_ok;
        }
        else
        {
            return diag_err_value;
        }
    }
    else if(mode == diag_mode_read)
    {
        snprintf(val, DIAG_VAL_BUF_LEN, "%i", cfg.wifi_power_on_state);

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


static diag_err_t diag_ota_url(char const * key, char * const val, diag_mode_t mode)
{
    return diag_handle_string(key, val, mode, cfg.ota_url, CFG_NODE_NAME_LEN);
}


static diag_err_t diag_keys(char const * key, char * const val, diag_mode_t mode);



/* Table mapping service keys to service implementations. */
diag_tbl_t diag_service_tbl[] =
{
    { "stime", diag_sensor_timer },
    { "sprint", diag_sensor_print },
    { "reset", diag_do_reset },
    { "uptime", diag_uptime },
    { "cfgload", diag_load_cfg },
    { "cfgsave", diag_save_cfg },
    { "wifiname", diag_wifi_name },
    { "wifipwd", diag_wifi_password },
    { "wifipon", diag_wifi_pon_connect },
    { "wifistat", diag_wifi_status },
    { "nodename", diag_node_name },
    { "wifi", diag_wifi_state },
    { "diagkeys", diag_keys },
    { "otaurl", diag_ota_url },
    { {'\0'}, NULL },
};

static diag_err_t diag_keys(char const * key, char * const val, diag_mode_t mode)
{
    if(mode == diag_mode_read)
    {
        for(diag_tbl_t *e = &(diag_service_tbl[0]); e->svc_fct != NULL; ++e)
        {
            Serial.print(F("dda:"));
            Serial.println(e->key);
        }

        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}
