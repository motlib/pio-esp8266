#ifndef LED_TYPES_H
#define LED_TYPES_H

/**
 * Runtime data for blinking / flashing led.
 */
typedef struct
{
    uint8_t flash_count;
    uint8_t state;
    uint16_t cnt;
} led_data_t;

typedef struct
{
    uint8_t pin;
    led_data_t * data;
    uint16_t flash_on_time;
    uint16_t flash_off_time;
    uint16_t flash_wait_time;
} led_cfg_t;



#endif /* LED_TYPES_H */
