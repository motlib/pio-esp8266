/**
 * @file
 *
 * Application main entry file. This is Arduino style, so no main() function,
 * but setup() and loop().
 */

#include <Arduino.h>

#include "cfg/cfg.h"
#include "led.h"
#include "net/httpsrv.h"
#include "net/telnet.h"
#include "net/wifi.h"
#include "sensor.h"
#include "system.h"
#include "term/serterm.h"
#include "term/term.h"
#include "uptime.h"
#include "net/mqtt.h"
#include "net/thingspeak.h"

/**
 * Cycle time of the main loop [10ms].
 */
#define MAIN_CYCLE_TIME 10


/**
 * Arduino style setup function. This is called once and initializes the system.
 */
void setup()
{
    Serial.begin(115200);

    term_init(&serterm_desc);
    term_put_line(&serterm_desc, "i:booting");

    telnet_init();
    term_init(&telnet_term_desc);
    
    /* Initialize and load the configuration. */
    cfg_init();
    (void)cfg_load();

    sensor_init();
    httpsrv_init();
    wifi_init();
    mqtt_init();
    ts_init();
    
    led_init(&led_stat);

    
    term_put_line(&serterm_desc, "i:booted");
}


static void main_tasks(void)
{
    system_main();

    term_main(&serterm_desc);
    term_main(&telnet_term_desc);

    uptime_main();
    wifi_main();
    httpsrv_main();
    telnet_main();
    sensor_main();
    led_main(&led_stat);

    mqtt_main();
    ts_main();
}


/**
 * Arduino style loop function. This is called cyclically and runs the main
 * software.
 *
 * The loop function should not run for too long, as between the calls to this
 * function, the arduino framework calls the internal functions (mainly
 * networking stack) of the esp8266 hardware.
 */
void loop()
{
    static uint32_t max_time = 0;

    /* Run task functions and measure time. */
    
    /* TODO: this only measures the time of the application tasks, but not the
     * time of the background esp tasks like wifi and tcp operation. */
    
    uint32_t start = millis();
    main_tasks();
    uint32_t end = millis();

    /* This even works with overflows of the millis counter. :-) */
    uint32_t time = end - start;
    if(max_time < time)
    {
        max_time = time;
        Serial.print(F("i:max_loop_time="));
        Serial.println(max_time);
    }

    /* If we did not use up all the available time, we delay a bit. */
    if(time < MAIN_CYCLE_TIME)
    {
        delay(MAIN_CYCLE_TIME - time);
    }
    else
    {
        /* ok, we took too long for the last loop. So no delay and directly
         * start again. */
    }
}
