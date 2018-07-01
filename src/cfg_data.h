#ifndef CFG_DATA_H
#define CFG_DATA_H


#include <Arduino.h>
#include <stdint.h>

typedef struct
{
    /* Network config */
    char wifi[32];
    char password[32];

    /* sensor cycle time */
    uint16_t sens_cycle_time;


    /* Checksum protection of the eeprom data. */
    uint16_t crc16;
} cfg_data_t;


extern cfg_data_t cfg;

extern cfg_data_t const cfg_defaults PROGMEM;


#endif /* CFG_DATA_H */
