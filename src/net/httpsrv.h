/**
 * @file
 *
 * Module to implement a web server to serve status and sensor data of the node.
 */

#ifndef HTTPSRV_H
#define HTTPSRV_H

#include <ESP8266WebServer.h>


/**
 * Global HTTP server instance. Shall be only accessed from inside the httpsrv
 * modules.
 */
extern ESP8266WebServer httpsrv;


/**
 * Initialize the HTTP server. 
 */
void httpsrv_init(void);


/**
 * Main function to operate the HTTP server.
 */
void httpsrv_main(void);


#endif /* HTTPSRV_H */
