#ifndef CFG_H
#define CFG_H

#include <stdint.h>

#include "cfg_data.h"


#define CFG_LOAD_OK 0
#define CFG_LOAD_CRC_ERROR 1


void cfg_init(void);

uint8_t cfg_load(void);

void cfg_save(void);


#endif /* CFH_H */
