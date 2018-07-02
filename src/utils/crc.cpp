#include "crc.h"

#define CRC_MSB_MASK 0x8000

/* XModem polynomial */
#define CRC_POLY 0x1021


#define CRC_INIT 0xffffu


static void crc16_update(uint16_t * const crc, uint8_t const data)
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


/**
 * Calculate a CRC16 checksum. 
 *
 * For specific variant of crc16, check the CRC_POLY and CRC_INIT defines.
 * 
 * @param data Pointer to the data.
 * @param size Size of the data, i.e. the number of bytes.
 *
 * @returns The crc16 checksum of the data.
 */
uint16_t crc16(uint8_t const * data, size_t size)
{
    uint16_t crc = CRC_INIT;

    while(size > 0)
    {
        crc16_update(&crc, *data);
        data++;
        size--;
    }

    return crc;
}
