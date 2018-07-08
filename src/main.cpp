/**
 * @file
 *
 * Application main entry file. This is Arduino style, so no main() function,
 * but setup() and loop().
 */

#include <Arduino.h>

#include "system.h"
#include "sensor.h"
#include "term/term.h"
#include "term/serterm.h"
#include "uptime.h"
#include "cfg/cfg.h"
#include "wifi.h"
#include "httpsrv.h"
#include "net/telnet.h"

/**
 * Cycle time of the main loop.
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

    term_put_line(&serterm_desc, "i:booted");
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

    uint32_t start = millis();

    system_main();
    term_main(&serterm_desc);
    uptime_main();
    wifi_main();
    httpsrv_main();
    telnet_main();
    term_main(&telnet_term_desc);

    uint32_t end = millis();

    /* This even works with overflows of the millis counter. :-) */
    uint32_t time = end - start;

    if(max_time < time)
    {
        max_time = time;
        Serial.print(F("i:max_loop_time="));
        Serial.println(max_time);
    }

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
