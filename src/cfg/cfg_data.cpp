#include "cfg_data.h"

#include <Arduino.h>

/**
 * Ram shadow variable for loading configuration data from EEPROM.
 */
cfg_data_t cfg;


/**
 * Default data for configuration. 
 */
cfg_data_t const cfg_defaults PROGMEM =
{
    /* general system data */
    .node_name = {0},

    /* Network data */
    .wifi = {0},
    .password = {0},

    /* Read sensor cycle time in [10ms]. */
    .sens_cycle_time = 500,

    /* config checksum. Value is not relevant, as it is calculated before
     * writing to EEPROM. */
    .crc16 = 0x0u,
};

