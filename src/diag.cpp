#include "diag.h"

#include <Arduino.h>
#include <string.h>

#include "system.h"


#define DIAG_REQ_BUF_LEN 64

#define DIAG_SEPARATOR '='

typedef struct
{
    char req_buf[DIAG_REQ_BUF_LEN];
    uint8_t req_idx;
    diag_err_t err;
} diag_data_t;


static diag_data_t diag_data = {
    .req_buf = { 0 },
    .req_idx = 0,
    .err = diag_err_ok,
};

static String data("");

static diag_err_t diag_set_timer(char const * key, char const * val)
{
    int time = atoi(val);
            
    if((time >= 50) && (time <= (5 * 60 * 100)))
    {
        sys_config.sens_cycle_time = time;
        
        return diag_err_ok;
    }
    else
    {
        return diag_err_value;
    }
}

static diag_err_t diag_handle_input()
{
    char * const sep = index(diag_data.req_buf, DIAG_SEPARATOR);
    if(sep == NULL)
    {
        return diag_err_input;
    }

    /* replace separator by 0 charater to terminate key string. */
    *sep = '\0';

    char const * const key = diag_data.req_buf;
    char const * const val = (sep + 1);

    if(strcmp(key,"time") == 0)
    {
        return diag_set_timer(key, val);
    }
    else
    {
        return diag_err_key;
    }

    return diag_err_ok;
}

void diag_main()
{
    while(Serial.available() > 0)
    {
        int c = Serial.read();

        /* Depending on the connected terminal, we receive CR or LF, so we
         * handle both as end-of-command. */
        if((c == '\r') || (c == '\n'))
        {
            diag_err_t result = diag_handle_input();

            Serial.print(F("RESPONSE=0x"));
            Serial.println(result, HEX);
            
            diag_data.req_idx = 0;
            diag_data.req_buf[0] = '\0';
        }
        else
        {
            if(diag_data.req_idx >= DIAG_REQ_BUF_LEN - 2)
            {
                diag_data.err = diag_err_input;
            }
            else
            {
                
                /* Add received character to buffer and always terminate with \0. */
                diag_data.req_buf[diag_data.req_idx] = (char)c;
                diag_data.req_idx++;
                diag_data.req_buf[diag_data.req_idx] = '\0';
            }
        }
    }   
}
