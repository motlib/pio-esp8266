#include "cfg_data.h"

#include <Arduino.h>

cfg_data_t cfg;

cfg_data_t const cfg_defaults PROGMEM = {
    .wifi = {0},
    .password = {0},

    /* Read sensor cycle time in [10ms]. */
    .sens_cycle_time = 500,

    /* Init crc, but is not relevant, as it is calculated before writing to
     * EEPROM. */
    .crc16 = 0x0u,
};

