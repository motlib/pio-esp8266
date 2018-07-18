
#include "sensorstat.h"
#include "uptime.h"
#include "sensor.h"

#define SENSORSTAT_BUF_INTERVAL (60 * 5)

#define SENSORSTAT_BUF_LEN 12


typedef struct
{
    float buf[SENSORSTAT_BUF_LEN];
    uint8_t initialized;
    float trend_1h;
} sensorstat_data_t;

static sensorstat_data_t sensorstat_data =
{
    .buf = {0},
    .initialized = 0,
    .trend_1h = 0,
};

void sensorstat_main(void)
{
    uint32_t uptime = uptime_get_seconds();
    uint8_t i;

    if(!sensorstat_data.initialized)
    {
        // ... and if sensor data available
        for(i = 0; i < SENSORSTAT_BUF_LEN; ++i)
        {
            sensorstat_data.buf[i] = sensor_get_temp();
        }

        sensorstat_data.initialized = 1;
    }

    
    if(uptime % (SENSORSTAT_BUF_INTERVAL) == 0)
    {
        /* shift buffer to clear index 0 and add new value. */
        for(i = 0; i < SENSORSTAT_BUF_LEN - 1; ++i)
        {
            sensorstat_data.buf[i+1] = sensorstat_data.buf[i];
        }
        
        sensorstat_data.buf[0] = sensor_get_temp();
    }

    /* As the buffer contains one hour of data, we do not need to scale to have
     * trend in [change/hour]. */
    sensorstat_data.trend_1h =
        (sensorstat_data.buf[0] - sensorstat_data.buf[SENSORSTAT_BUF_LEN - 1])
        / SENSORSTAT_BUF_LEN / SENSORSTAT_BUF_INTERVAL * 3600;
}

float sensorstat_get_trend(void)
{
    return sensorstat_data.trend_1h;
}
        

    
