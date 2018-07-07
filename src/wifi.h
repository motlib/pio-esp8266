#ifndef WIFI_H
#define WIFI_H

#include <stdint.h>
#include "diag/diag_services.h"


/* States */
#define WIFI_OFFLINE 0u
#define WIFI_GO_ONLINE 1u
#define WIFI_ONLINE 2u


void wifi_init(void);
void wifi_main(void);
void wifi_request_state(uint8_t state);
uint8_t wifi_get_state(void);

diag_err_t diag_wifi_status(char const * key, char * const val, diag_mode_t mode);


#endif /* WIFI_H */
