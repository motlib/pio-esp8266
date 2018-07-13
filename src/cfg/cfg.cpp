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

#include <EEPROM.h>
#include <stdint.h>

#include "cfg.h"
#include "cfg/cfg_cfg.h"
#include "utils/crc.h"
#include "utils/det.h"


void cfg_init(void)
{
    EEPROM.begin(CFG_EEP_SIZE);
}


uint8_t cfg_load(void)
{
    DET_ASSERT(sizeof(cfg_data_t) < CFG_EEP_SIZE);

    EEPROM.get(CFG_ADDR, cfg);

    uint16_t crc = crc16((uint8_t*)&cfg, sizeof(cfg_data_t) - 2);

    /* If the crc does not match, we copy the defaults. */
    if(crc != cfg.crc16)
    {
        /* Attention: normal memcpy will fail of the data is not aligned for 32
         * bit access. */
        memcpy_P(&cfg, &cfg_defaults, sizeof(cfg_data_t));
        
        return CFG_LOAD_CRC_ERROR;
    }
    
    return CFG_LOAD_OK;
}


void cfg_save(void)
{
    /* Update crc before writing to eeprom */
    cfg.crc16 = crc16((uint8_t*)&cfg, sizeof(cfg_data_t) - 2);
    
    EEPROM.put(CFG_ADDR, cfg);
    
    EEPROM.commit();
}
