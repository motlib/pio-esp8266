/**
 * @file
 *
 * Module to measure the uptime since power-on.
 */

#include <Arduino.h>
#include <stdint.h>

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


/**
 * Main function for the uptime counter component.
 *
 * Needs to be called regularly (e.g. every 10ms or 100ms) to update the uptime
 * counter. Does not need to be called with exact timing, as the timebase is
 * used from the Aruino millis() timer value.
 */
void uptime_main(void)
{
    uint32 cnt = millis();
    
    /* this even works, when the counter overflows. */
    uptime_data.msecs += (cnt - uptime_data.last_cnt);

    /* Store current counter value for next cycle. */
    uptime_data.last_cnt = cnt;
    
    /* Add full seconds to second counter. */
    uptime_data.secs += uptime_data.msecs / 1000;
    
    /* Subtract full seconds and keep remainder */
    uptime_data.msecs %= 1000;
}


/**
 * Get the current uptime (time since power-on) in seconds.
 * 
 * @returns Seconds since power-on. 32 bits is good for up to 49710 days or 136
 *   years.
 */
uint32 uptime_get_seconds(void)
{
    return uptime_data.secs;
}
