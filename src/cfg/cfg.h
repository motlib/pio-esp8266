/**
 * @file
 *
 * Handler for storing and loading the system configuration. The configuration
 * is stored in EEPROM (emulation).
 *
 * Access to the configuration is provided by the global cfg structure defined
 * in cfg_data.cpp.
 *
 * The configuration data is protected by a 16 bit CRC checksum. When loading
 * the configuration, the checksum is checked. If it is invalid, default data
 * from ROM is used instead.
 */

#ifndef CFG_H
#define CFG_H

#include <stdint.h>

#include "cfg_data.h"


/** Return value for cfg_load(): data loaded successfully from EEPROM. */
#define CFG_LOAD_OK 0

/** Return value for cfg_load(): Data in EEPROM not valid, default data used. */
#define CFG_LOAD_CRC_ERROR 1


/**
 * Initialize the configuration handler.
 */
void cfg_init(void);


/**
 * Load the configuration from EEPROM to the RAM shadow.
 *
 * After load, the crc16 checksum of the configuration is checked. If it is not
 * valid, default values from ROM are used instead.
 *
 * @retval CFG_LOAD_OK if the configuration has been loaded from EEPROM.
 * @retval CFG_LOAD_CRC_ERROR if the default values from ROM have been used.
 */
uint8_t cfg_load(void);


/**
 * Store the configuration to EEPROM. 
 *
 * This calculates the crc16 over the configuration data and then writes to
 * EEPROM.
 */
void cfg_save(void);


#endif /* CFH_H */
