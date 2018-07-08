#ifndef TERM_H
#define TERM_H


#include <stdint.h>

/**
 * Terminal descriptor data type.
 *
 * This is used to specify a command-line terminal. The descriptor holds the
 * function pointers to the necessary functionality (read a character, write a
 * character, handle user input).
 */
typedef struct
{
    /**
     * Read input.
     * @returns < 0 for no input available.
     */
    int (*get_char)(void);
    /**
     * Print a character to the output.
     */
    void (*put_char)(char c);

    /**
     * Handle the input after the user entered a whole line.
     */
    void (*line_handler)(char * line);

    /** Terminal line buffer */
    char *buf;

    /** Length of the line buffer. */
    uint16_t buf_len;
    
    /** Pointer to the next free position in buf. */
    uint8_t idx;

} term_desc_t;

void term_init(term_desc_t * const term_desc);
void term_main(term_desc_t * const term_desc);

void term_put_str(term_desc_t const * const term_desc, char const * str);


#endif /* TERM_H */
