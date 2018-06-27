
#include <Arduino.h>

#include "system.h"

void setup()
{

    Serial.begin(115200);
    Serial.println(F("BME280 test"));

    sensor_init();
}





void loop()
{
    system_main();
    diag_main();

    delay(10);
}


