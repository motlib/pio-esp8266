#ifndef CFG_DATA_H
#define CFG_DATA_H


#include <Arduino.h>
#include <stdint.h>


/**
 * Maximum length of the node name.
 */
#define CFG_NODE_NAME_LEN 32 

/**
 * Maximum length of the wifi name.
 */
#define CFG_WIFI_NAME_LEN 32

/**
 * Maximum length of the wifi password.
 */
#define CFG_WIFI_PWD_LEN 32


/**
 * Configuration data type.
 */
typedef struct
{
    char node_name[CFG_NODE_NAME_LEN];

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
