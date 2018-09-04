/**
 * @file
 *
 * Module to measure the uptime since power-on.
 */

#include <stdint.h>
#include "uptime.h"    

#ifndef UPTIME_COUNTER
#include <Arduino.h>

#define UPTIME_COUNTER millis
#endif

#ifndef UPTIME_COUNTER_SEC_SCALE
#define UPTIME_COUNTER_SEC_SCALE 1000
#endif


/**
 * Runtime data of the uptime counter.
 */
typedef struct
{
    /** Seconds since power-on. 32 bits is good for up to 49710 days or 136
     * years. */
    uint32_t secs;
    
    /** Milli-seconds (combined with secs above) since power-on */
    uint32_t msecs;

    /** Last value of the internal millis() time counter. */
    uint32_t last_cnt;
} uptime_data_t;


/* Internal data of the uptime counter. */
static uptime_data_t uptime_data = {
    .secs = 0,
    .msecs = 0,
    .last_cnt = 0,
};


/* Update the time counter. */
void uptime_main(void)
{
    uint32_t cnt = UPTIME_COUNTER();
    
    /* this even works, when the millis() counter overflows. */
    uptime_data.msecs += (cnt - uptime_data.last_cnt);

    /* Store current counter value for next cycle. */
    uptime_data.last_cnt = cnt;
    
    /* Add full seconds to second counter. */
    uptime_data.secs += uptime_data.msecs / UPTIME_COUNTER_SEC_SCALE;
    
    /* Subtract full seconds and keep remainder */
    uptime_data.msecs %= UPTIME_COUNTER_SEC_SCALE;
}


/* Return full seconds since power-on. */
uint8_t uptime_get_seconds(uint32_t * const val)
{
    *val = uptime_data.secs;

    return VFCT_ERR_OK;
}


const vfct_t uptime_seconds_vfct = VFCT_DEF(u32, uptime_get_seconds, NULL);
