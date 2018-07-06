#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "uptime.h"

#define BME280_MY_ADDRESS (0x76)


typedef struct {
    float temp;
    float pres;
    float hum;
} sensor_data_t;

static sensor_data_t sensor_data;


static Adafruit_BME280 bme; // I2C




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

void sensor_sample()
{
    sensor_data.temp = bme.readTemperature();
    sensor_data.pres = bme.readPressure() / 100.0f;
    sensor_data.hum = bme.readHumidity();
}



void sensor_print_values()
{
#ifdef SENSOR_PRINT_UPTIME
    Serial.print(F("Uptime = "));
    Serial.print(uptime_get_seconds());
    Serial.println(F("s"));
#endif /* SENSOR_PRINT_UPTIME */
    
    Serial.print(F("temperature="));
    Serial.print(sensor_data.temp);
    Serial.println(F(" *C"));

    Serial.print(F("pressure="));
    Serial.print(sensor_data.pres);
    Serial.println(F("hPa"));

    Serial.print("humidity = ");
    Serial.print(sensor_data.hum);
    Serial.println("%");

    Serial.println();
}

float sensor_get_temp(void)
{
    return sensor_data.temp;
}

float sensor_get_pres(void)
{
    return sensor_data.pres;
}

float sensor_get_hum(void)
{
    return sensor_data.hum;
}
