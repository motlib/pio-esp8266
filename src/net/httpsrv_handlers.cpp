#include "cfg/cfg.h"
#include "httpsrv.h"
#include "httpsrv_handlers.h"
#include "sensor.h"
#include "uptime.h"
#include "utils/det.h"


/* Server scratch pad size */
#define HTTPSRV_BUF_LEN 128


/* Server scratch pad buffer. */
static char buf[HTTPSRV_BUF_LEN];


/**
 * HTTP handler for /data.
 *
 * Prints some node information and the last sampled sensor data.
 */
static void httpsrv_handle_data(void)
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
        "node=%s\ntemp=%.2f\nhum=%.2f\npressure=%.2f\nuptime=%u\n",
        cfg_wifi.node_name,
        t,
        h,
        p,
        uptime);

    /* Ensure that the scratch pad is big enough. */
    DET_ASSERT(n < HTTPSRV_BUF_LEN - 1);

    httpsrv.send(200, "text/plain", buf);
}


/**
 * HTTP handler for root document (/).
 */
static void httpsrv_handle_root(void)
{
    /* TODO: send better response on root path */
    
    httpsrv.send(200, "text/plain", "Hello world");
}


/**
 * Register handler functions with http server component.
 */
void httpsrv_register_handlers(void)
{
    httpsrv.on("/", httpsrv_handle_root);
    httpsrv.on("/data", httpsrv_handle_data);
}
