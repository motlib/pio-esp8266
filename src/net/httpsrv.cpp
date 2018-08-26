/**
 * @file
 *
 * This module operates the HTTP server and defines the functions to send
 * response data to requests.
 */

#include "httpsrv.h"
#include "httpsrv_cfg.h"
#include "net/httpsrv_handlers.h"
#include <ESP8266WebServer.h>


/* Web server instance to listen on port 80. */
ESP8266WebServer httpsrv(HTTPSRV_PORT);



/* Register request handlers and start the server */
void httpsrv_init(void)
{
    httpsrv_register_handlers();
    
    httpsrv.begin();
}


/* Handle a connected client */
void httpsrv_main(void)
{
    httpsrv.handleClient();
}
