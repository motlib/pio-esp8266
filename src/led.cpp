
#include "led.h"
#include "led_cfg.h"

/* pinMode, digitalWrite */
#include <Arduino.h>

void led_init(void)
{
    pinMode(LED_PIN, OUTPUT);
}


void led_main(void)
{
    static uint8_t state;
    static uint16_t cnt;

    if(cnt > 0)
    {
        --cnt;
    }
    else
    {
        if(state == 0)
        {
            // switch on led
            digitalWrite(LED_PIN, LOW);
            cnt = LED_ON_TIME;
            state = 1;
        }
        else
        {
            digitalWrite(LED_PIN, HIGH);
            cnt = LED_OFF_TIME;
            state = 0;
        }
        
    }
}
