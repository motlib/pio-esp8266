/**
 * @file
 *
 * This module implements the diagnosis component. It receives text commands
 * from the terminal and processes them. 
 *
 * Commands and responses are always in the format "key=value".
 */

#include "diag.h"
#include "diag_cfg.h"

#include <Arduino.h>
#include <string.h>

#include "system.h"
#include "diag_services.h"

/* TODO: correctly handle terminal input and output. Curently terminal input and
 * serial output are mixed. */


static diag_tbl_t* diag_find_service(char const * const key)
{
    diag_tbl_t *entry = NULL;
    int i = 0;
    
    for(entry = &(diag_service_tbl[i]);
        entry->svc_fct != NULL;
        i++, entry = &(diag_service_tbl[i]))
    {
        if(strcmp(entry->key, key) == 0)
        {
            return entry;
        }
    }

    return NULL;
}    


static diag_err_t diag_handle_request(char * line_buf)
{
    /* find separator character in request */
    char * sep = NULL;
    diag_mode_t mode;
    
    if((sep = index(line_buf, DIAG_WRITE_CHAR)) != NULL)
    {
        mode = diag_mode_write;
    }
    else if((sep = index(line_buf, DIAG_READ_CHAR)) != NULL)
    {
        mode = diag_mode_read;
    }
    else
    {
        return diag_err_input;
    }
 
    /* replace separator by 0 charater to terminate key string. */
    *sep = '\0';

    char const * const key = line_buf;
    char * const val = (sep + 1);

    diag_tbl_t *entry = diag_find_service(key);
    if(entry == NULL)
    {
        return diag_err_key;
    }

    diag_err_t err = entry->svc_fct(key, val, mode);

    /* In read mode, the returned value is placed into the value buffer. */
    if((err == diag_err_ok) && (mode == diag_mode_read))
    {
        Serial.print(key);
        Serial.print('=');
        Serial.println(val);
    }
    
    return err;
}

/**
 * Handler to process a received diagnosis command. 
 */
void diag_handle_input(char * line_buf)
{
    diag_err_t err = diag_handle_request(line_buf);

    Serial.print(F("dresp="));
    Serial.println(err, HEX);
}
