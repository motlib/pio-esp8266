/**
 * @file
 * 
 * Application main entry file. This is Arduino style, so no main() function,
 * but setup() and loop().
 */

#include <Arduino.h>


#include "system.h"
#include "sensor.h"
#include "diag/diag.h"
#include "uptime.h"
#include "cfg/cfg.h"
#include "wifi.h"
#include "httpsrv.h"


/**
 * Arduino style setup function. This is called once and initializes the system.
 */
void setup()
{
    Serial.begin(115200);

    Serial.print(F("i:booting..."));
    
    /* Initialize and load the configuration. */
    cfg_init();
    
    uint8_t err = cfg_load();
    Serial.print(F("i:cfg_load="));
    Serial.println(err);
    
    sensor_init();

    httpsrv_init();
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
    static uint32_t last_cnt = millis();
    uint32_t next_cnt;
    uint32_t curr;


    uint32_t start = millis();
    
    system_main();
    diag_main();
    uptime_main();
    wifi_main();
    httpsrv_main();

    uint32_t end = millis();

    /* This even works with overflows of the millis counter. :-) */
    uint32_t t = end - start;

    if(t < 10)
    {
        delay(10-t);
    }
    else
    {
        /* ok, we took too long. */
        Serial.print("loop_time=");
        Serial.println(t);
    }    
}


