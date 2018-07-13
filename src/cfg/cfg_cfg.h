/**
 * @file
 * 
 * Pre-compile time configuration of the configuration manager component.
 */

#ifndef CFG_CFG_H
#define CFG_CFG_H

/**
 * Size of the eeprom area. There is no real eeprom available on ESP8266, but
 * data is stored in one flash page.
 */
#define CFG_EEP_SIZE 512


/**
 * Address where to store the configuration in EEPROM. As we only have one
 * configuration struct, it's located at address 0.
 */
#define CFG_ADDR 0

#endif /* CFG_CFG_H */
