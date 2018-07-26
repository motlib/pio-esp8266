/**
 * @file
 *
 * Simple uptime counter module for Arduino framework.  Uses the millis()
 * function to determine uptime, so its main function does not need to be called
 * in fixed intervals.
 */ 

#ifndef UPTIME_H
#define UPTIME_H


#include <stdint.h>

/**
 * Main function for the uptime counter component.
 *
 * Needs to be called regularly (e.g. every 10ms or 100ms) to update the uptime
 *  counter. Does not need to be called with exact timing, as the timebase is
 * used from the Aruino millis() timer value.
 */
void uptime_main(void);


/**
 * Get the current uptime (time since power-on) in seconds.
 * 
 * @returns Seconds since power-on. 32 bits is good for up to 49710 days or 136
 * years.
 */
uint32_t uptime_get_seconds(void);


#endif /* UPTIME_H */
