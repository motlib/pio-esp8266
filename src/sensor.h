#ifndef SENSOR_H
#define SENSOR_H


void sensor_init(void);
void sensor_print_values(void);
void sensor_sample(void);

float sensor_get_temp(void);
float sensor_get_pres(void);
float sensor_get_hum(void);

#endif /* SENSOR_H */
