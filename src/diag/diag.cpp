/**
 * @file
 *
 * This module implements the diagnosis component. It receives text commands
 * from the terminal and processes them. 
 *
 * request:
 *   <key>? | <key>=<value>
 * if the service needs to reply with much data: 
 *   dd:<response_data>
 * for reading
 * 


 */

#include "diag.h"
#include "diag_cfg.h"

#include <Arduino.h>
#include <string.h>

#include "system.h"
#include "diag_services.h"

#include "term/term.h"

/* TODO: correctly handle terminal input and output. Curently terminal input and
 * serial output are mixed. */

/**
 * Pointer to the currently active terminal for diagnostics i/o.
 */
static term_desc_t const * diag_term = NULL;


/**
 * Find the pointer to the diagnostic service in the service table.
 *
 * @param key The service key (name) to look up.
 *
 * @return Pointer to the diagnostic service in the service table or NULL, if no
 *   service can be found.
 */
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
        diag_print_data(val);
        
        //Serial.print(key);
        //Serial.print('=');
        //Serial.println(val);
    }
    
    return err;
}


/**
 * Handler to process a received diagnosis command. 
 */
void diag_handle_input(term_desc_t const * const desc)
{
    if(diag_term == NULL)
    {
        diag_term = desc;
    }
    else
    {
        if(diag_term != desc)
        {
            // TODO: log out last session
            diag_term = desc;
        }
    }
    
    diag_err_t err = diag_handle_request(desc->buf);

    char buf[12];
    snprintf(buf, 12, "dresp:0x%x", err);
    
    term_put_line(desc, buf);
}


void diag_print_data(char * data)
{
    if(diag_term != NULL)
    {
        term_put_str(diag_term, "dd:");
        term_put_line(diag_term, data);
    }
}
