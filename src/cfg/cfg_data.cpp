/**
 * @file
 * 
 * This module defines the global instance of the configuration data cfg and the
 * default data to be used if configuration data in EEPROM is not valid.
 */

#include "cfg_data.h"
#include <pgmspace.h>


/* Ram shadow variable for loading configuration data from EEPROM. */
cfg_data_t cfg;

/* Ram shadow variable for loading wifi configuration data from EEPROM. */
cfg_wifi_t cfg_wifi;


/* Default data for configuration. */
cfg_data_t const cfg_defaults PROGMEM =
{    
    /* Read sensor cycle time in [10ms]. */
    .sens_cycle_time = 500u,

    /* Print sensor values to serial interface */
    .sens_print = 1u,

    /* Over-the-air update server. */
    .ota_host = {0},
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


cfg_block_t const cfg_block_tbl[CFG_BLOCK_COUNT] =
{
    {
        .shadow = &cfg_wifi,
        .defaults = &cfg_wifi_defaults,
        .size = sizeof(cfg_wifi),
    },
    {
        .shadow = &cfg,
        .defaults = &cfg_defaults,
        .size = sizeof(cfg),
    }
};
