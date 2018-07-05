#ifndef WIFI_H
#define WIFI_H

#include <stdint.h>

/* States */
#define WIFI_OFFLINE 0u
#define WIFI_GO_ONLINE 1u
#define WIFI_ONLINE 2u


void wifi_main(void);
void wifi_request_state(uint8_t state);
uint8_t wifi_get_state(void);


#endif /* WIFI_H */
