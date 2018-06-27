# pio-esp8266

This is (yet another) experiment with a ESP8266 module (a Wemos Mini D1) and a
Bosch BME280 temperature, huminidty and pressure sensor. This time, the
development platform is PlatformIO instead of pure Arduino.

## Development

To compile the software, you need to go to the root directory of the project
(this directory) and run

* `pipenv install` to install the platformio core toolchain.
* `pipenv shell` to start a Python environment to run the following commands
  (PlatformIO is Python 2.7 based).
* `pio run` to build the firmware and / or `pio run -t upload` to build and
  upload the firmware to a ESP8266 module.
  
  


