#ifndef CFG_DATA_H
#define CFG_DATA_H


#include <Arduino.h>
#include <stdint.h>


#define CFG_WIFI_NAME_LEN 32
#define CFG_WIFI_PWD_LEN 32

typedef struct
{
    /* Network config */
    char wifi[CFG_WIFI_NAME_LEN];
    char password[CFG_WIFI_PWD_LEN];

    /* sensor cycle time */
    uint16_t sens_cycle_time;


    /* Checksum protection of the eeprom data. */
    uint16_t crc16;
} cfg_data_t;


extern cfg_data_t cfg;

extern cfg_data_t const cfg_defaults PROGMEM;


#endif /* CFG_DATA_H */
