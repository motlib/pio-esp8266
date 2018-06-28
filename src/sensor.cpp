#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "uptime.h"

#define SEALEVELPRESSURE_HPA (1013.25)

#undef BME280_ADDRESS
#define BME280_MY_ADDRESS (0x76)


Adafruit_BME280 bme; // I2C


void sensor_init(void)
{
    Wire.begin();


    Wire.beginTransmission(BME280_MY_ADDRESS);
    uint8_t error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.print(F("OK: Sensor acknowledges address "));
    }
    else
    {
        Serial.print(F("ERROR: Sensor does not acknowledge I2C address "));
    }
    Serial.print(BME280_MY_ADDRESS, HEX);

    
    bme.begin(BME280_MY_ADDRESS);
    
    if(bme.init() == false)
    {
        Serial.print(F("ERROR: Sensor init failed. Code: "));
        Serial.println(error);
        
        while(1);
    }

    bme.setSampling(
        Adafruit_BME280::MODE_NORMAL, // cyclic sampling
        Adafruit_BME280::SAMPLING_X16, // temperature
        Adafruit_BME280::SAMPLING_X16, // pressure
        Adafruit_BME280::SAMPLING_X16, // humidity
        Adafruit_BME280::FILTER_OFF);

}


void sensor_print_values()
{
    Serial.print(F("Uptime = "));
    Serial.print(uptime_get_seconds());
    Serial.println(F("s"));
    
    Serial.print(F("Temperature = "));
    Serial.print(bme.readTemperature());
    Serial.println(F(" *C"));

    Serial.print(F("Pressure = "));
    Serial.print(bme.readPressure() / 100.0f);
    Serial.println(F("hPa"));

    //Serial.print("Approx. Altitude = ");
    //Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    //Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println("%");

    Serial.println();
}

