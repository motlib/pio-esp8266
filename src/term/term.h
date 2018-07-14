/**
 * @file
 *
 * Implementation of a simple terminal / command-line interface. 
 */

#ifndef TERM_H
#define TERM_H


#include <stdint.h>

/** A linefeed */
#define TERM_LINEFEED "\r\n"


/**
 * Macro to print a string on the terminal, automatically adding a line-feed.
 */
#define term_put_line(DESC, STR) \
    term_put_str((DESC), STR);   \
    term_put_str((DESC), TERM_LINEFEED)


typedef struct s_term_desc_t term_desc_t;

/**
 * Terminal descriptor data type.
 *
 * This is used to specify a command-line terminal. The descriptor holds the
 * function pointers to the necessary functionality (read a character, write a
 * character, handle user input).
 */
struct s_term_desc_t
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
    void (*line_handler)(term_desc_t const * const desc);

    /**
     * Terminal line buffer
     */
    char *buf;

    /**
     * Length of the line buffer.
     */
    uint16_t buf_len;
    
    /**
     * Pointer to the next free position in buf. This will be managed by the
     * terminal implementation.
     */
    uint8_t idx;

};


void term_init(term_desc_t * const term_desc);
void term_main(term_desc_t * const term_desc);

void term_put_str(term_desc_t const * const term_desc, char const * str);




#endif /* TERM_H */
