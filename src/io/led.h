/**
 * @file
 *
 * Header for led control module. Idea is to control a LED to flash fast in a
 * sequence and than have a longer interval where it's off. This makes it easy
 * to count the flashes and indicate any kind of status.
 */

#ifndef LED_H
#define LED_H

#include <stdint.h>

#include "led_types.h"
#include "led_cfg.h"

/**
 * Initialize the LED.
 *
 * This initializes the LED i/o port.
 *
 * @param[in] led Pointer to the LED configuration / data structure.
 */
void led_init(led_cfg_t const * led_cfg);

/**
 * Run the led task.
 *
 * This regularly flashes the LED, so it must be called with a regular time
 * interval, e.g. every 10ms.
 *
 * @param[in] led Pointer to the LED configuration / data structure.
*/
void led_main(led_cfg_t const * led_cfg);

/**
 * Set the flash count state of the led.#
 *
 * A flash count of 0 switches off the LED completely.
 *
 * @param[in] led Pointer to the LED configuration / data structure.
 * @param flash_count Number of flashes.
 */
void led_set_flash_count(led_cfg_t const * led_cfg, uint8_t flash_count);

#endif /* LED_H */
