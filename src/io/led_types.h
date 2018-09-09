/**
 * @file
 *
 * Datatypes to control led operation.
 */

#ifndef LED_TYPES_H
#define LED_TYPES_H

/**
 * Runtime data for blinking / flashing led.
 */
typedef struct
{
    /** Number of flashes to show. */
    uint8_t flash_count;
    /** Current state of the led (led on / off / long delay). */
    uint8_t state;

    /** Time counter to measure how long a state of the led shall be
     * active. [10ms] */
    uint16_t cnt;
} led_data_t;


/**
 * Led confoguration structure.
 */
typedef struct
{
    /** The i/o pin to control (Arduino numbering) */
    uint8_t pin;
    /** Pointer to the runtime data structure. */
    led_data_t * data;

    /** Time while the led is on [led main function calls] */
    uint16_t flash_on_time;

    /** Time while the led is off [led main function calls] */    
    uint16_t flash_off_time;

    /** Time while the led is off waiting for the next flash cycle [led main
     * function calls] */
    uint16_t flash_wait_time;
} led_cfg_t;



#endif /* LED_TYPES_H */
