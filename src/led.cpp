#include "led.h"
#include "led_cfg.h"

/* pinMode, digitalWrite */
#include <Arduino.h>



void led_init(led_cfg_t const * led)
{
    pinMode(led->pin, OUTPUT);
}


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


void led_main(led_cfg_t const * led)
{
    if(led->data->cnt > 0)
    {
        --led->data->cnt;
    }
    else
    {
        if(led->data->state >= (2 * led->data->flash_count))
        {
            /* wait for flash interval */
            led->data->cnt = led->flash_wait_time;

            /* after waiting, start with flashing again. */
            led->data->state = 0u;
            
            /* switch off led */
            digitalWrite(led->pin, HIGH);
        }
        else if(led->data->state % 2 == 0u)
        {
            /* wait for flash interval */
            led->data->cnt = led->flash_on_time;
            led->data->state += 1;

            digitalWrite(led->pin, LOW);

        }
        else
        {
            led->data->cnt = led->flash_off_time;
            led->data->state += 1;
            
            digitalWrite(led->pin, HIGH);
        }
    }
}
