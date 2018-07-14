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

/* TODO: Add support for multiple blocks (e.g. wifi and other configuration)*/


void cfg_init(void)
{
    EEPROM.begin(CFG_EEP_SIZE);
}

static uint8_t cfg_validate_block(cfg_block_t const * blk)
{
    uint16_t crc = crc16((uint8_t*)blk->shadow, blk->size - 2);

    /* pointer to the crc16 member (last two bytes) */
    uint8_t * crcptr = ((uint8_t*)blk->shadow) + blk->size - 2;
    uint16_t shwcrc = *(uint16_t*)crcptr;
    
    /* If the crc does not match, we copy the defaults. */
    if(crc != shwcrc)
    {
        /* Attention: normal memcpy will fail of the data is not aligned for 32
         * bit access. */
        memcpy_P(blk->shadow, blk->defaults, blk->size);
        
        return CFG_LOAD_CRC_ERROR;
    }

    return CFG_LOAD_OK;
}


uint8_t cfg_load(void)
{
    //DET_ASSERT(sizeof(cfg_data_t) < CFG_EEP_SIZE);

    cfg_block_t const * blk;
    uint16_t addr = 0;
    uint8_t *eep_data = EEPROM.getDataPtr();
    uint8_t status = 0;
    
    for(uint8_t i = 0; i < CFG_BLOCK_COUNT; ++i)
    {
        blk = &(cfg_block_tbl[i]);
        /* EEPROM class already contains a shadow. This is a temporary
         * workaround until configuration blocks are accessed as pointers. */
        memcpy(blk->shadow, eep_data + addr, blk->size);

        status |= cfg_validate_block(blk);

        addr += blk->size;
        
        DET_ASSERT(addr < CFG_EEP_SIZE);
    }        

    return status;
}


void cfg_save(void)
{
    cfg_block_t const * blk;
    uint16_t addr = 0;
    uint8_t *eep_data = EEPROM.getDataPtr();

    for(uint8_t i = 0; i < CFG_BLOCK_COUNT; ++i)
    {
        blk = &(cfg_block_tbl[i]);

        uint8_t * crcptr = ((uint8_t*)blk->shadow) + blk->size - 2;
        
        /* Update crc before writing to eeprom */
        *(uint16_t*)crcptr = crc16((uint8_t*)blk->shadow, blk->size - 2);

        memcpy(eep_data + addr, blk->shadow, blk->size);

        addr += blk->size;

        DET_ASSERT(addr < CFG_EEP_SIZE);
    }
   
    EEPROM.commit();
}
