/**
 * @file
 *
 * This module implements the diagnosis component. It receives text commands
 * from the serial interface and processes them. 
 *
 * Commands and responses are always in the format "key=value".
 */
#include "diag.h"
#include "diag_cfg.h"

#include <Arduino.h>
#include <string.h>

#include "system.h"
#include "diag_services.h"


//typedef struct
//{
//    /** Diagnostic request buffer */
//    char req_buf[DIAG_REQ_BUF_LEN];
//    /** Pointer to the next free position in req_buf. */
//    uint8_t req_idx;
//    /** Error flag */
//    diag_err_t err;
//} diag_data_t;


//static diag_data_t diag_data = {
//    .req_buf = { 0 },
//    .req_idx = 0,
//    .err = diag_err_ok,
//};


/**
 * Handler to process a received diagnosis command. 
 */
void diag_handle_input(char * line_buf)
{
    /* Check if we already had an error (e.g. input overflow error). */
    //if(diag_data.err != diag_err_ok)
    //{
    //    return diag_data.err;
    //}

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
        return;// diag_err_input;
    }
 
    /* replace separator by 0 charater to terminate key string. */
    *sep = '\0';

    char const * const key = line_buf;
    char * const val = (sep + 1);

    diag_tbl_t *entry;
    int i = 0;
    diag_err_t err;
    for(entry = &(diag_service_tbl[i]);
        entry->svc_fct != NULL;
        i++, entry = &(diag_service_tbl[i]))
    {
        if(strcmp(entry->key, key) == 0)
        {
            err = entry->svc_fct(key, val, mode);
            if((err == diag_err_ok) && (mode == diag_mode_read))
            {
                Serial.print(key);
                Serial.print('=');
                Serial.println(val);
            }
            
            //err;
            break;
        }
    }

    err = diag_err_key;

    Serial.print(F("RESPONSE=0x"));
    Serial.println(err, HEX);
}


///**
// * Main method of diagnosis component. 
// */
//void diag_main()
//{
//    while(Serial.available() > 0)
//    {
//        int c = Serial.read();
//
//        /* Depending on the connected terminal, we receive CR or LF, so we
//         * handle both as end-of-command. */
//        if((c == '\r') || (c == '\n'))
//        {
//            diag_err_t result = diag_handle_input();
//
//            Serial.print(F("RESPONSE=0x"));
//            Serial.println(result, HEX);
//
//            /* reset request buffer */
//            diag_data.req_idx = 0;
//            diag_data.req_buf[0] = '\0';
//        }
//        else
//        {
//            if(diag_data.req_idx >= DIAG_REQ_BUF_LEN - 2)
//            {
//                diag_data.err = diag_err_input_length;
//            }
//            else
//            {
//                
//                /* Add received character to buffer and always terminate with \0. */
//                diag_data.req_buf[diag_data.req_idx] = (char)c;
//                diag_data.req_idx++;
//                diag_data.req_buf[diag_data.req_idx] = '\0';
//            }
//        }
//    }
//}
//
