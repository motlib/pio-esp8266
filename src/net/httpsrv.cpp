/**
 * @file
 *
 * This module operates the HTTP server and defines the functions to send
 * response data to requests.
 */

#include "httpsrv.h"

#include <ESP8266WebServer.h>
#include "sensor.h"
#include "uptime.h"

static ESP8266WebServer server(80);

#define HTTPSRV_BUF_LEN 128

static char buf[HTTPSRV_BUF_LEN];


/**
 * HTTP handler for /data.
 *
 * Prints the last sampled sensor data.
 */
static void handleData(void)
{
    snprintf(
        buf,
        HTTPSRV_BUF_LEN,
        "temp=%.2f\nhum=%.2f\npressure=%.2f\nuptime=%i\n",
        sensor_get_temp(),
        sensor_get_hum(),
        sensor_get_pres(),
        uptime_get_seconds());

    server.send(200, "text/plain", buf);
}


/**
 * HTTP handler for /.
 */
static void handleRoot(void)
{
    server.send(200, "text/plain", "Hello world");
}


/**
 * Initialize the HTTP server. 
 */
void httpsrv_init(void)
{
    server.on("/", handleRoot);
    server.on("/data", handleData);
    
    server.begin();
}


/**
 * Main function to operate the HTTP server.
 */
void httpsrv_main(void)
{
    server.handleClient();
}
