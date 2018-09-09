#include "led.h"
#include "led_cfg.h"

/* pinMode, digitalWrite */
#include <Arduino.h>


/*
 * Configure the led port.
 */
void led_init(led_cfg_t const * led)
{
    pinMode(led->pin, OUTPUT);
}


/*
 * Change the flash count in the led runtime data structure. If the flash count
 * changes, the flashing cycle is restarted.
 */
void led_set_flash_count(led_cfg_t const * led, uint8_t flash_count)
{
    if(flash_count != led->data->flash_count)
    {
        led->data->flash_count = flash_count;
    
        /* Set state to off and counter expired, so that led is switched on in
         * next cycle. */
        led->data->state = 0;
        led->data->cnt = 0;
    }
}


/*
 * Operate the led statmachine. Even state number to switch the led on, odd
 * state numbers to switch it off. Last state (equals two times flash count) to
 * wait for longer delay between flash sequences.
 */
void led_main(led_cfg_t const * led)
{
    /* Wait until the counter for the current led state is expired. */
    if(led->data->cnt > 0)
    {
        --led->data->cnt;
        return;
    }

    if(led->data->state >= (2 * led->data->flash_count))
    {
        /* wait for flash pause */
        led->data->cnt = led->flash_wait_time;

        /* after waiting, start with flashing again. */
        led->data->state = 0u;
        
        /* switch off led */
        digitalWrite(led->pin, HIGH);
    }
    else if(led->data->state % 2 == 0u)
    {
        /* wait for led on time */
        led->data->cnt = led->flash_on_time;
        led->data->state += 1;

        digitalWrite(led->pin, LOW);
    }
    else
    {
        /* wait for led off time */
        led->data->cnt = led->flash_off_time;
        led->data->state += 1;
        
        digitalWrite(led->pin, HIGH);
    }
}
