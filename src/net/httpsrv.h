/**
 * @file
 *
 * Module to implement a web server to serve status and sensor data of the node.
 */

#ifndef HTTPSRV_H
#define HTTPSRV_H

/** TCP port where the http server listens. */
#define HTTPSRV_PORT 80


/**
 * Initialize the HTTP server. 
 */
void httpsrv_init(void);

/**
 * Main function to operate the HTTP server.
 */
void httpsrv_main(void);

#endif /* HTTPSRV_H */
