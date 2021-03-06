#ifndef DIAG_SERVICES_H
#define DIAG_SERVICES_H

#include "diag_cfg.h"

/**
 * Diagnostic service return values (error codes).
 */
typedef enum
{
    /** No error. */
    diag_err_ok = 0,
    
    /** Invalid input. */
    diag_err_input,
    
    /** Input too long (request buffer overflow) */
    diag_err_input_length,
    
    /** Unknown service key received. */
    diag_err_key,
    
    /** Invalid value received. */
    diag_err_value,

    /** Mode (read or write) is not supported. */
    diag_err_mode_unsupported
} diag_err_t;


/**
 * Diagnostic service mode, i.e. read or write service.
 */
typedef enum
{
    /** Diagnostic service to read a value from the device. */
    diag_mode_read,
    /** Diagnostic service to write a value to the device. */
    diag_mode_write,
} diag_mode_t;


/**
 * Diagnostic service table entry structure.
 */
typedef struct
{
    /**
     * Key (name) of the service.
     */
    char key[DIAG_KEY_BUF_LEN];

    /**
     * Diagnostic service function pointer.
     *
     * @param key Service key (name)
     * @param value Value to set
     * @param mode Defines if the service is called for reading or writing.
     */
    diag_err_t (*svc_fct)(
        char const * const key,
        char * const value,
        diag_mode_t const mode,
        void * const extra_data);

    /**
     * Extra data to pass to the service implementation. Usage and expected data
     * type is depending on service implementation.
     */
    void * const extra_data;
} diag_tbl_t;


/**
 * Diagnostic service table.
 *
 * Last entry is marked by a NULL pointer in the srv_svc member.
 */
extern diag_tbl_t const diag_service_tbl[];

#endif /* DIAG_SERVICES_H */
