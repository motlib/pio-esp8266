#ifndef DIAG_H
#define DIAG_H

#include "term/term.h"


/**
 * Length of the value buffer of diagnostic service requests / responses.
 */
#define DIAG_VAL_BUF_LEN (DIAG_REQ_BUF_LEN - DIAG_KEY_BUF_LEN)


/**
 * This function handles a line of input from the terminal and processes it
 * (e.g. by executing the diagnostic service).
 */
void diag_handle_input(term_desc_t const * const desc);


/**
 * Function to print data to the terminal while executing a diagnostic service.
 *
 * @attention This function must only be called while a diafgnostic service
 *   function is executed.
 */
void diag_print_data(char const * data);


#endif /* DIAG_H */
