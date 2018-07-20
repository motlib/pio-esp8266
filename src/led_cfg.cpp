#include "led.h"
#include "led_cfg.h"
#include "led_types.h"

/* LED_BUILTIN */
#include <Arduino.h>

static led_data_t led_stat_data =
{
    .flash_count = 0,
    .state = 0,
    .cnt = 0,
};


led_cfg_t const led_stat =
{
    .pin = LED_BUILTIN,
    .data = &led_stat_data,
    .flash_on_time = 1,
    .flash_off_time = 40,
    .flash_wait_time = 500,
};
