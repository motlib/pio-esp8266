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
    system_main();
    diag_main();
    uptime_main();
    wifi_main();

    delay(10);
}


