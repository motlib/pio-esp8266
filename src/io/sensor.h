#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

#include "diag/diag_services.h"
#include "utils/vfct.h"


/**
 * Initialize the sensor. This function must be called before calling any other
 * sensor function. It reads the calibration data from the sensor for later
 * use.
 */
void sensor_init(void);

/**
 * Read the current values from the sensor.
 */
void sensor_sample(void);

/**
 * Read the sensor temperature value. This value is updated when calling
 * sensor_sample().
 */
uint8_t sensor_get_temp(float * const t);

/**
 * Read the sensor temperature value. This value is updated when calling
 * sensor_sample().
 */
uint8_t sensor_get_pres(float * const t);

/**
 * Read the sensor temperature value. This value is updated when calling
 * sensor_sample().
 */
uint8_t sensor_get_hum(float * const t);


/**
 * Diagnostic interface to the the sensor information.
 */
diag_err_t sensor_diag_info(char const * key, char * const val, diag_mode_t mode, void * const extra_data);


/* vfct interface to the sensor settings / values. */

extern vfct_t const sensor_vfct_timer;
extern vfct_t const sensor_vfct_temp;
extern vfct_t const sensor_vfct_pres;
extern vfct_t const sensor_vfct_hum;



#endif /* SENSOR_H */
