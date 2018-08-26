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
#include "cfg/cfg.h"
#include "utils/det.h"


/* Web server instance to listen on port 80. */
static ESP8266WebServer server(HTTPSRV_PORT);

/* Server scratch pad size */
#define HTTPSRV_BUF_LEN 128

/* Server scratch pad buffer. */
static char buf[HTTPSRV_BUF_LEN];


/**
 * HTTP handler for /data.
 *
 * Prints some node information and the last sampled sensor data.
 */
static void handleData(void)
{
    int n;
    float t, h, p;
    uint32_t uptime;

    (void)sensor_get_temp(&t);
    (void)sensor_get_hum(&h);
    (void)sensor_get_pres(&p);
    (void)uptime_get_seconds(&uptime);
    
    n = snprintf(
        buf,
        HTTPSRV_BUF_LEN,
        "node=%s\ntemp=%.2f\nhum=%.2f\npressure=%.2f\nuptime=%i\n",
        cfg_wifi.node_name,
        t,
        h,
        p,
        uptime);

    /* Ensure that the scratch pad is big enough. */
    DET_ASSERT(n < HTTPSRV_BUF_LEN - 1);

    server.send(200, "text/plain", buf);
}


/**
 * HTTP handler for root document (/).
 */
static void handleRoot(void)
{
    /* TODO: send better response on root path */
    
    server.send(200, "text/plain", "Hello world");
}


/* Register request handlers and start the server */
void httpsrv_init(void)
{
    server.on("/", handleRoot);
    server.on("/data", handleData);
    
    server.begin();
}


/* Handle a connected client */
void httpsrv_main(void)
{
    server.handleClient();
}
