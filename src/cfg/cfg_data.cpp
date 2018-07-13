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


/* Default data for configuration. */
cfg_data_t const cfg_defaults PROGMEM =
{
    /* general system data */
    .node_name = {0},

    /* Network data */
    .wifi_name = {0},
    .wifi_password = {0},
    .wifi_power_on_state = 1,
    
    /* Read sensor cycle time in [10ms]. */
    .sens_cycle_time = 500u,

    /* Print sensor values to serial interface */
    .sens_print = 1u,

    /* config checksum. Value is not relevant, as it is calculated before
     * writing to EEPROM. */
    .crc16 = 0x0u,
};

