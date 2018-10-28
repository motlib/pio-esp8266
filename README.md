# pio-esp8266

This is (yet another) experiment with a ESP8266 module (a Wemos D1 Mini) and a
Bosch BME280 temperature, huminidty and pressure sensor. This time, the
development platform is PlatformIO instead of pure Arduino.


## References

* Wemos [D1 Mini Wiki Page]
* Bosch [BME280 Product Page] and [BME280 Datasheet]


## Development

To compile the software, you need to go to the root directory of the project
(this directory) and run

* `pipenv install` to install the platformio core toolchain.
* `pipenv shell` to start a Python environment to run the following commands
  (PlatformIO is Python 2.7 based).
* `pio run` to build the firmware and / or `pio run -t upload` to build and
  upload the firmware to a ESP8266 module.
  
  
## Hardware

# Serial Connection

The serial connection to access the diagnosis interface is running with 115200 
baud. 

# I2C Connection to BME280

The connection to the BME280 sensor is using I2C protocol, which uses 4
wires. Usually there are two additional pins on the BME280 breakout board, which
remain unconnected. 

| BME280 | Wemos D1 Mini |
|--------+---------------|
| VCC    | 3V3           |
| GND    | GND           |
| SCL    | D1            |
| SDA    | D2            |
| CSB    | not connected |
| SDO    | not connected |

[D1 Mini Wiki Page]: https://wiki.wemos.cc/products:d1:d1_mini
[BME280 Product Page]: https://www.bosch-sensortec.com/bst/products/all_products/bme280
[BME280 Datasheet]: https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS002.pdf
