/**
 * @file
 * Header for led control module.
 */

#ifndef LED_H
#define LED_H

/**
 * Initialize the LED.
 */
void led_init(void);

/**
 * Run the led task. This regularly blinks the LED.
 */
void led_main(void);

#endif /* LED_H */
