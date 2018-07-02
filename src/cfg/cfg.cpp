/**
 * @file
 *
 * Handler for storing and loading the system configuration. The configuration
 * is stored in EEPROM (emulation).
 */

#include <EEPROM.h>
#include <stdint.h>

#include "cfg.h"
#include "utils/crc.h"
#include "utils/det.h"

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


/**
 * Initialize the configuration handler.
 */
void cfg_init(void)
{
    EEPROM.begin(CFG_EEP_SIZE);
}


/**
 * Load the configuration from EEPROM to the RAM shadow.
 *
 * After load, the crc16 checksum of the configuration is checked. If it is not
 * valid, default values from ROM are used instead.
 *
 * @retval CFG_LOAD_OK if the configuration has been loaded from EEPROM.
 * @retval CFG_LOAD_CRC_ERROR if the default values from ROM have been used.
 */
uint8_t cfg_load(void)
{
    DET_ASSERT(sizeof(cfg_data_t) < CFG_EEP_SIZE);

    EEPROM.get(CFG_ADDR, cfg);

    uint16_t crc = crc16((uint8_t*)&cfg, sizeof(cfg_data_t) - 2);

    /* If the crc does not match, we copy the defaults. */
    if(crc != cfg.crc16)
    {
        cfg = cfg_defaults;

        return CFG_LOAD_CRC_ERROR;
    }

    return CFG_LOAD_OK;
}


/**
 * Store the configuration to EEPROM. 
 *
 * This calculates the crc16 over the configuration data and then writes to
 * EEPROM.
 */
void cfg_save(void)
{
    /* Update crc before writing to eeprom */
    cfg.crc16 = crc16((uint8_t*)&cfg, sizeof(cfg_data_t) - 2);
    
    EEPROM.put(CFG_ADDR, cfg);
    
    EEPROM.commit();
}
