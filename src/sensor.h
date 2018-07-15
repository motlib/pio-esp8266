#ifndef SENSOR_H
#define SENSOR_H

#include "diag/diag_services.h"

void sensor_init(void);
void sensor_main(void);
void sensor_sample(void);

float sensor_get_temp(void);
float sensor_get_pres(void);
float sensor_get_hum(void);

diag_err_t diag_sensor_info(char const * key, char * const val, diag_mode_t mode);

#endif /* SENSOR_H */
