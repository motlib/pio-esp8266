/**
 * @file
 *
 * Header for the configuration data specification.
 *
 * This file declares the data structure for configuration data and the global
 * variable instance to provide configuration data access in the application.
 */

#ifndef CFG_DATA_H
#define CFG_DATA_H


/* PROGMEM macro */
#include <pgmspace.h>

/* Standard integer datatypes */
#include <stdint.h>


/** Maximum length of the node name. */
#define CFG_NODE_NAME_LEN 32 

/** Maximum length of the wifi name. */
#define CFG_WIFI_NAME_LEN 32

/** Maximum length of the wifi password. */
#define CFG_WIFI_PWD_LEN 32

/** Maximum length of the OTA update url. */
#define CFG_OTA_URL_LEN 128


/**
 * Configuration data type.
 */
typedef struct
{
    /** Node name of this node. */
    char node_name[CFG_NODE_NAME_LEN];

    
    /** Wifi network name */
    char wifi_name[CFG_WIFI_NAME_LEN];

    /** Wifi network password. */
    char wifi_password[CFG_WIFI_PWD_LEN];

    /** Required wifi state after power-on. (==0: no wifi, !=0: wifi enabled) */
    uint8_t wifi_power_on_state;

    
    /** Sensor read cycle time (in [10ms], i.e. main loop cycle time). */
    uint16_t sens_cycle_time;
    
    /** Print sensor values to serial interface. */
    uint8_t sens_print;

    /** over-the-air update URL. */
    char ota_url[CFG_OTA_URL_LEN];
    
    /** CRC16 checksum protection of the eeprom data. */
    uint16_t crc16;
} cfg_data_t;


/**
 * Configuration data available for access in all modules.
 */
extern cfg_data_t cfg;


/**
 * Default values for configuration.
 *
 * This is used, if the checksum of the configuration data in EEPROM is not
 * valid.  This variable shall only be used internally in the cfg module.
 */
extern cfg_data_t const cfg_defaults PROGMEM;


#endif /* CFG_DATA_H */
