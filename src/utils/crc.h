/**
 * @file
 *
 * Utility functions to calculate crc checksums.
 */

#ifndef CRC_H
#define CRC_H

/* Integer datatypes */
#include <stdint.h>
/* size_t */
#include <stddef.h>


uint16_t crc16(uint8_t const * data, size_t size);


#endif /* CRC_H */
