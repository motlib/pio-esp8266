#include "cfg.h"


#include <EEPROM.h>

#include <stdint.h>


#define CFG_EEP_SIZE 512

#define CFG_ADDR 0


#define CRC_MSB_MASK 0x8000

/* XModem polynomial */
#define CRC_POLY 0x1021

#define CRC_STATE 0xffffu


static void crc16_update(uint16_t * crc, uint8_t data)
{
    int j = 0x80;

    while (j > 0)
    {
        uint16_t bit = (uint16_t)(*crc & CRC_MSB_MASK);
		
        *crc <<= 1;

        if((data & j) != 0)
        {
            bit = (uint16_t)(bit ^ CRC_MSB_MASK);
        }
        
        if (bit != 0)
        {
            *crc ^= CRC_POLY;
        }
        
        j >>= 1;
    }
}


static uint16_t crc16(uint8_t const * data, uint32_t size)
{
    uint16_t crc = 0xffffu;

    while(size > 0)
    {
        crc16_update(&crc, *data);
        data++;
        size--;
    }

    return crc;
}




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
