#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

#include "diag/diag_services.h"
#include "utils/vfct.h"


void sensor_init(void);
void sensor_main(void);
//void sensor_sample(void);

uint8_t sensor_get_temp(float * const t);
uint8_t sensor_get_pres(float * const t);
uint8_t sensor_get_hum(float * const t);

diag_err_t sensor_diag_info(char const * key, char * const val, diag_mode_t mode, void * const extra_data);


extern vfct_t const sensor_vfct_timer;
extern vfct_t const sensor_vfct_temp;
extern vfct_t const sensor_vfct_pres;
extern vfct_t const sensor_vfct_hum;



#endif /* SENSOR_H */
