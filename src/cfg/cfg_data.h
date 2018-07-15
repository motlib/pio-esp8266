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


#define CFG_BLOCK_COUNT 2


/** Maximum length of the node name. */
#define CFG_NODE_NAME_LEN 32 

/** Maximum length of the wifi name. */
#define CFG_WIFI_NAME_LEN 32

/** Maximum length of the wifi password. */
#define CFG_WIFI_PWD_LEN 32

/** Maximum length of the OTA update host. */
#define CFG_OTA_HOST_LEN 32

/** Maximum length of the OTA update url path. */
#define CFG_OTA_PATH_LEN 32


typedef struct {
    void * shadow;
    void const * defaults;
    uint8_t size;
} cfg_block_t;


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

    /** CRC16 checksum protection of the eeprom data. */
    uint16_t crc16;
} cfg_wifi_t;


/**
 * Configuration data type.
 */
typedef struct
{
    /** Sensor read cycle time (in [10ms], i.e. main loop cycle time). */
    uint16_t sens_cycle_time;
    
    /** Print sensor values to serial interface. */
    uint8_t sens_print;

    /** over-the-air update host. */
    char ota_host[CFG_OTA_HOST_LEN];

    /** over-the-air update server port */
    uint16_t ota_port;
    
    /** over-the-air update URL. */
    char ota_path[CFG_OTA_PATH_LEN];
    
    /** CRC16 checksum protection of the eeprom data. */
    uint16_t crc16;
} cfg_data_t;


/**
 * Configuration for wifi. Kept separate, so this block stays valid after update
 * of cfg_data_t configuration structure.
 */
extern cfg_wifi_t cfg_wifi;


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
extern cfg_wifi_t const cfg_wifi_defaults PROGMEM;


extern cfg_block_t const cfg_block_tbl[CFG_BLOCK_COUNT];

#endif /* CFG_DATA_H */
