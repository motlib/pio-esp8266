/**
 * @file
 *
 * Compile-time configuration of the diagnosis module.
 */

#ifndef DIAG_CFG_H
#define DIAG_CFG_H

/** Length of the diagnostic request / response buffer. */
#define DIAG_REQ_BUF_LEN (64u)

/** Maximum length of the key part of the diag request. */
#define DIAG_KEY_BUF_LEN (10u)

/** Charater for value assignment (separator between key and value). */
#define DIAG_WRITE_CHAR '='

/** Character for value reading (last character of the request) */
#define DIAG_READ_CHAR '?'


#endif /* DIAG_CFG_H */
