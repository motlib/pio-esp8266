#ifndef WIFI_H
#define WIFI_H

#include <stdint.h>
#include "diag/diag_services.h"
#include "utils/vfct.h"

/* States */
#define WIFI_OFFLINE 0u
#define WIFI_GO_ONLINE 1u
#define WIFI_ONLINE 2u


void wifi_init(void);
void wifi_main(void);
void wifi_request_state(uint8_t state);
uint8_t wifi_get_state(void);
diag_err_t wifi_diag_status(char const * key, char * const val, diag_mode_t mode, void * const extra_data);

extern vfct_t const wifi_vfct_rssi;


#endif /* WIFI_H */
