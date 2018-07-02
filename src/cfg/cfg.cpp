#include "cfg.h"


#include <EEPROM.h>
#include <stdint.h>

#include "utils/crc.h"


#define CFG_EEP_SIZE 512

#define CFG_ADDR 0






void cfg_init(void)
{
    EEPROM.begin(CFG_EEP_SIZE);
}


uint8_t cfg_load(void)
{
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


void cfg_save(void)
{
    /* Update crc before writing to eeprom */
    cfg.crc16 = crc16((uint8_t*)&cfg, sizeof(cfg_data_t) - 2);
    
    EEPROM.put(CFG_ADDR, cfg);
    
    EEPROM.commit();
}
