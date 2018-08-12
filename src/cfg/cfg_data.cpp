/**
 * @file
 * 
 * This module defines the global instance of the configuration data cfg and the
 * default data to be used if configuration data in EEPROM is not valid.
 */

#include "cfg_data.h"
#include <pgmspace.h>


/* Ram shadow variable for general configuration data. */
cfg_data_t cfg;

/* Ram shadow variable for wifi configuration data. */
cfg_wifi_t cfg_wifi;

/* Ram shadow variable for MQTT configuration data. */
cfg_mqtt_t cfg_mqtt;


/* Default data for configuration. */
cfg_data_t const cfg_defaults PROGMEM =
{    
    /* Read sensor cycle time in [10ms]. */
    .sens_cycle_time = 500u,

    /* Print sensor values to serial interface */
    .sens_print = 1u,

    /* Over-the-air update server. */
    .ota_host = {0},
    /* Over-the-air update port. */    
    .ota_port = 80,
    /* Over-the-air update URL path. */
    .ota_path = {0},

    /* config checksum. Value is not relevant, as it is calculated before
     * writing to EEPROM. */
    .crc16 = 0x0u,
};


cfg_wifi_t const cfg_wifi_defaults PROGMEM =
{
    /* general system data */
    .node_name = {0},

    /* Network data */
    .wifi_name = {0},
    .wifi_password = {0},
    .wifi_power_on_state = 1u,

    .crc16 = 0x0u,
};    



cfg_mqtt_t const cfg_mqtt_defaults PROGMEM =
{
    /* MQTT broker config */
    .broker = {0},
    .port = 1883,
    .user = {0},
    .password = {0},

    /* Thingspeak data */
    .ts_channel = {0},
    .ts_channel_key = {0},

    .crc16 = 0x0u,
};



/* Table of config blocks */
cfg_block_t const cfg_block_tbl[CFG_BLOCK_COUNT] =
{
    /* wifi settings */
    {
        .shadow = &cfg_wifi,
        .defaults = &cfg_wifi_defaults,
        .size = sizeof(cfg_wifi),
    },
    
    /* general settings */
    {
        .shadow = &cfg,
        .defaults = &cfg_defaults,
        .size = sizeof(cfg),
    },
    
    /* MQTT settings */
    {
        .shadow = &cfg_mqtt,
        .defaults = &cfg_mqtt_defaults,
        .size = sizeof(cfg_mqtt),
    }
};
