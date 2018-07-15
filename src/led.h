/**
 * @file
 * Header for led control module.
 */

#ifndef LED_H
#define LED_H

#include <stdint.h>

#include "led_cfg.h"


/**
 * Initialize the LED.
 */
void led_init(void);

/**
 * Run the led task. This regularly blinks the LED.
 */
void led_main(void);

void led_set(uint16_t on_time, uint16_t off_time);

#endif /* LED_H */
