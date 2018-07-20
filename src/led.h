/**
 * @file
 *
 * Header for led control module.
 */

#ifndef LED_H
#define LED_H

#include <stdint.h>

#include "led_types.h"
#include "led_cfg.h"

/**
 * Initialize the LED.
 */
void led_init(led_cfg_t const * led_cfg);

/**
 * Run the led task. This regularly blinks the LED.
 */
void led_main(led_cfg_t const * led_cfg);

/**
 * Set the state of the led.
 */
void led_set_flash_count(led_cfg_t const * led_cfg, uint8_t flash_count);

#endif /* LED_H */
