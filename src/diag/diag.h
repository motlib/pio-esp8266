#ifndef DIAG_H
#define DIAG_H

#define DIAG_REQ_BUF_LEN (64)

#define DIAG_KEY_BUF_LEN (8)

#define DIAG_VAL_BUF_LEN (DIAG_REQ_BUF_LEN - DIAG_KEY_BUF_LEN)



/**
 * Main method of diagnosis component.
 *
 * Call cyclically to process incoming diagnosis requests.
 */
void diag_main();


#endif /* DIAG_H */
