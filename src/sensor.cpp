#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>


#define SEALEVELPRESSURE_HPA (1013.25)


#define BME280_ADDRESS (0x76)
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C


void sensor_init(void)
{
    Wire.begin();


    Wire.beginTransmission(BME280_ADDRESS);
    uint8_t error = Wire.endTransmission();
    if (error == 0)
    {
        Serial.println(F("OK: Sensor acknowledges address."));
    }
    else
    {
        Serial.print(F("ERROR: Sensor does not acknowledge I2C address."));
    }
    
    bme.begin(BME280_ADDRESS);
    error = bme.init();
    if(error)
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
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    //Serial.print("Approx. Altitude = ");
    //Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    //Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}

