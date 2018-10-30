# pio-esp8266

This is (yet another) experiment with a ESP8266 module (a Wemos D1 Mini) and a
Bosch BME280 temperature, huminidty and pressure sensor. This time, the
development platform is PlatformIO instead of pure Arduino.

Main idea is to create a simple sensor node, which reports its sensor values
(temperature, humidity, air pressure) over MQTT. It shall support a simple
command-line interface, i.e. no wifi access point and website, but a serial and
telnet based terminal for configuration, called diagnostic interface *).

*) I work in automotive elecronics development, so I took over some ideas from
ECU development. Here UDS and its services were the initial idea, but I later
decided for a human readable interface instead.


## Features

Implemented:

* Interface with BME280 sensor by I2C
* Implement simple HTTP based interface to check sensor state / values.
* Implement command-line based diagnostic interface, accessible by serial port
  (UART over USB) and telnet connection.
  
  
To be implemented:
  
* IN WORK: Publish sensor values and node state to a MQTT broker
* TODO: Implement sleep mode and other measures to reduce power consumption.
  
  
Remark to security: The node currently is designed to work in a safe local area
network (e.g. home network), so there are some security measures missing, which
would be needed in a public accessible network. E.g. OTA programming does not
check authenticity of the software, the diagnostic interface does not support
any login procedure, etc.


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

### Serial Connection

The serial connection to access the diagnosis interface is running with 115200 
baud. 

### I2C Connection to BME280

The connection to the BME280 sensor is using I2C protocol, which uses 4
wires. Usually there are two additional pins on the BME280 breakout board, which
remain unconnected. 

| BME280 | Wemos D1 Mini |
|--------|---------------|
| VCC    | 3V3           |
| GND    | GND           |
| SCL    | D1            |
| SDA    | D2            |
| CSB    | not connected |
| SDO    | not connected |

[D1 Mini Wiki Page]: https://wiki.wemos.cc/products:d1:d1_mini
[BME280 Product Page]: https://www.bosch-sensortec.com/bst/products/all_products/bme280
[BME280 Datasheet]: https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280_DS002.pdf


## Usage

### Diagnostic Interface

The diagnostic interface is a command-line interface to set and read parameters
of the node. You can connecto to the diagnostic interface by telnet (on port 23)
or serial connection (over USB). After connecting, you will see a command-prompt
where you can enter commands to read or write diagnostic data.

Commands are used to either read or write internal variables of the node. Some
variables only support reading or writing. 

To read a variable, enter its name like `variable`. E.g. to read the current
uptime of the node in seconds, you enter `uptime` like this (> marks the command
prompt). 

```text
> uptime
dd:42
```

The node will respond with the current uptime value. The prefix `dd:` is
prepended to mark diagnostic data.

To write a variable, enter its name followed by the equal character and the
value. E.g. this is how to set the wifi name:

```text
> wifi-name=MyWifiName
```

Some variables act as triggers for an action. E.g. to save the current
configuration to NvRAM, yopu write the value `1` to the `cfg-save` variable:

```text
> cfg-save=1
```

You can see the list of all available variables by entering `help`. 

When you change configuration variables, the change is only stored in
RAM. Depending on the variable, the new configuration is already used by the
node E.g. after setting the wifi name, the node uses the new name when
connecting the next time to wifi. 

The configuration changes will be lost when the node is powered off or reset. To
store the configuration in NvRAM, you need to use the `cfg-save` command.


### HTTP Interface

After the node has connected to the wifi network, you can access the sensor
readings by accessing `http://IPADDRESS/data` in text format.


