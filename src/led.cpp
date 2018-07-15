
#include "led.h"
#include "led_cfg.h"

/* pinMode, digitalWrite */
#include <Arduino.h>


#define LED_STATE_OFF 0
#define LED_STATE_ON 1


typedef struct
{
    uint16_t on_time;
    uint16_t off_time;
    uint8_t state;
    uint16_t cnt;
} led_data_t;

static led_data_t led_data = {
    .on_time = LED_ON_TIME,
    .off_time = LED_OFF_TIME,
    .state = 0,
    .cnt = 0,
};


void led_init(void)
{
    pinMode(LED_PIN, OUTPUT);
}


void led_set(uint16_t on_time, uint16_t off_time)
{
    led_data.on_time = on_time;
    led_data.off_time = off_time;
    
    /* Set state to off and counter expired, so that led is switched on in next
     * cycle. */
    led_data.state = LED_STATE_OFF;
    led_data.cnt = 0;
}

void led_main(void)
{
    if(led_data.cnt > 0)
    {
        --led_data.cnt;
    }
    else
    {
        if(led_data.state == LED_STATE_OFF)
        {
            // switch on led
            digitalWrite(LED_PIN, LOW);
            
            led_data.cnt = led_data.on_time;
            led_data.state = LED_STATE_ON;
        }
        else
        {
            digitalWrite(LED_PIN, HIGH);
            led_data.cnt = led_data.off_time;
            led_data.state = LED_STATE_OFF;
        }
        
    }
}
