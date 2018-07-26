/**
 * @file
 *
 * Config data handler component.
 */

#include <EEPROM.h>
#include <stdint.h>

#include "cfg.h"
#include "cfg/cfg_cfg.h"
#include "utils/crc.h"
#include "utils/det.h"

/* TODO: Pass config by pointer to apo, so that internal buffer of EEPROM can be used. */

/* Initialize the underlying EEPROM from Arsyino framework. */
void cfg_init(void)
{
    EEPROM.begin(CFG_EEP_SIZE);
}

/**
 * Check if the data read from nvram is valid by checking its CRC16.
 */
static uint8_t cfg_validate_block(cfg_block_t const * blk)
{
    uint16_t crc = crc16((uint8_t*)blk->shadow, blk->size - 2);

    /* pointer to the crc16 member (last two bytes) */
    uint8_t * crcptr = ((uint8_t*)blk->shadow) + blk->size - 2;
    uint16_t shwcrc = *(uint16_t*)crcptr;

    /* If the crc does not match, we copy the defaults. */
    if(crc != shwcrc)
    {
        /* Attention: normal memcpy will fail if the data is not aligned for 32
         * bit access. */
        memcpy_P(blk->shadow, blk->defaults, blk->size);

        return CFG_LOAD_CRC_ERROR;
    }

    return CFG_LOAD_OK;
}


/* 
 * Load data from EEPROM and check if valid. If not, then use default data from
 * ROM. 
 */
uint8_t cfg_load(void)
{
    uint16_t addr = 0;
    uint8_t *eep_data = EEPROM.getDataPtr();
    uint8_t status = 0;

    for(uint8_t i = 0; i < CFG_BLOCK_COUNT; ++i)
    {
        cfg_block_t const * blk = &(cfg_block_tbl[i]);

        /* EEPROM class already contains a shadow. This is a temporary
         * workaround until configuration blocks are accessed as pointers. */
        memcpy(blk->shadow, eep_data + addr, blk->size);

        status |= cfg_validate_block(blk);

        addr += blk->size;
        DET_ASSERT(addr < CFG_EEP_SIZE);
    }

    return status;
}


/* Calculate CRC of current config data and then store all config data in
 * nvram. */
void cfg_save(void)
{
    uint16_t addr = 0;
    uint8_t *eep_data = EEPROM.getDataPtr();

    for(uint8_t i = 0; i < CFG_BLOCK_COUNT; ++i)
    {
        cfg_block_t const * blk = &(cfg_block_tbl[i]);

        uint8_t * crcptr = ((uint8_t*)blk->shadow) + blk->size - 2;

        /* Update crc before writing to eeprom */
        *(uint16_t*)crcptr = crc16((uint8_t*)blk->shadow, blk->size - 2);

        memcpy(eep_data + addr, blk->shadow, blk->size);

        addr += blk->size;

        DET_ASSERT(addr < CFG_EEP_SIZE);
    }

    EEPROM.commit();
}
