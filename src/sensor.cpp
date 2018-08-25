#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "sensor.h"
#include "uptime.h"
#include "diag/diag.h"
#include "diag/diag_services.h"
#include "cfg/cfg.h"

#define BME280_MY_ADDRESS (0x76)

/* TODO: use my own BME280 lib */

#define SENS_ERR_NO_ACK 0x01
#define SENS_ERR_INIT_FAILED 0x02

typedef struct
{
    float temperature;
    float pressure;
    float humidity;
    uint8_t error_flags;
    uint16_t sample_timer;
} sensor_data_t;


/* Sensor runtime data. */
static sensor_data_t sensor_data =
{
    .temperature = 0.0f,
    .pressure = 0.0f,
    .humidity = 0.0f,
  
    .error_flags = 0x0u,

    .sample_timer = 0,
};


/* Sensor instance, by default uses I2C interface. */
static Adafruit_BME280 bme; 


/* Initialize data by checking sensor connectivity and read out calibration
 * data. Then configure sensor oversampling filter. */
void sensor_init(void)
{
    /* check if sensor is connected and responds with ACK */
    Wire.begin();
    Wire.beginTransmission(BME280_MY_ADDRESS);
    uint8_t error = Wire.endTransmission();
    if(error != 0)
    {
        sensor_data.error_flags |= SENS_ERR_NO_ACK;
    }

    /* read calibration data */
    bme.begin(BME280_MY_ADDRESS);
    
    if(bme.init() == false)
    {
        sensor_data.error_flags |= SENS_ERR_INIT_FAILED;
    }

    /* configure oversampling filter */
    if(sensor_data.error_flags == 0)
    {
        bme.setSampling(
            Adafruit_BME280::MODE_NORMAL, // cyclic sampling
            Adafruit_BME280::SAMPLING_X16, // temperature
            Adafruit_BME280::SAMPLING_X16, // pressure
            Adafruit_BME280::SAMPLING_X16, // humidity
            Adafruit_BME280::FILTER_OFF);
    }
}


/* Sample sensor data. */
static void sensor_sample()
{
    sensor_data.temperature = bme.readTemperature();
    sensor_data.pressure = bme.readPressure() / 100.0f;
    sensor_data.humidity = bme.readHumidity();
}


/* call every 10ms to sample sensor data according to sensor config. */
void sensor_main(void)
{
    if(sensor_data.sample_timer == 0)
    {
        sensor_data.sample_timer = cfg.sens_cycle_time;
        if(sensor_data.error_flags == 0)
        {
            sensor_sample();
        }
    }
    else
    {
        --sensor_data.sample_timer;
    }
}


/**
 * Diagnostic service to print current sensor values.
 */
diag_err_t diag_sensor_info(char const * key, char * const val, diag_mode_t mode)
{
    static const char * f_temp PROGMEM = "temperature=%.2f";
    static const char * f_pres PROGMEM = "pressure=%.2f";
    static const char * f_hum PROGMEM = "humidity=%.2f";
    static const char * f_err PROGMEM = "error_flags=%.2f";

    if(mode == diag_mode_read)
    {
        snprintf_P(val, DIAG_VAL_BUF_LEN, f_temp, sensor_data.temperature);
        diag_print_data(val);

        snprintf_P(val, DIAG_VAL_BUF_LEN, f_pres, sensor_data.pressure);
        diag_print_data(val);

        snprintf_P(val, DIAG_VAL_BUF_LEN, f_hum, sensor_data.humidity);
        diag_print_data(val);

        snprintf_P(val, DIAG_VAL_BUF_LEN, f_err, sensor_data.error_flags);
        diag_print_data(val);
        
        return diag_err_ok;
    }
    else
    {
        return diag_err_mode_unsupported;
    }
}


/* Get temperature value from sensor. */
uint8_t sensor_get_temp(float * const t)
{
    *t = sensor_data.temperature;

    return VFCT_ERR_OK;
}

/* Get pressure value from sensor. */
uint8_t sensor_get_pres(float * const p)
{
    *p = sensor_data.pressure;
    
    return VFCT_ERR_OK;
}

/* Get humidity value from sensor. */
uint8_t sensor_get_hum(float * const h)
{
    *h = sensor_data.humidity;

    return VFCT_ERR_OK;
}


uint8_t sensor_set_timer(uint32_t *t)
{
    /* TODO: range check! */
    if((*t >= 50) && (*t <= (5 * 60 * 100)))
    {
        cfg.sens_cycle_time = *t;

        return VFCT_ERR_OK;
    }
    else
    {
        return VFCT_ERR_RANGE;
    }
}


uint8_t sensor_get_timer(uint32_t *t)
{
    *t = cfg.sens_cycle_time;

    return VFCT_ERR_OK;
}

vfct_t const sensor_vfct_get_timer = VFCT_DEF(get_u32,sensor_get_timer);

vfct_t const sensor_vfct_set_timer = VFCT_DEF(set_u32,sensor_set_timer);


/* vfct descriptor to get sensor temperature value. */
vfct_t const sensor_vfct_temp = VFCT_DEF(get_f, sensor_get_temp);

/* vfct descriptor to get sensor pressure value. */
vfct_t const sensor_vfct_pres = VFCT_DEF(get_f, sensor_get_pres);

/* vfct descriptor to get sensor humidity value. */
vfct_t const sensor_vfct_hum = VFCT_DEF(get_f, sensor_get_hum);
