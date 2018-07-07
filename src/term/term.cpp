
#include "term.h"
#include "term_cfg.h"

#include <stdint.h>


typedef struct
{
    /** Terminal line buffer */
    char buf[TERM_BUF_LEN];
    /** Pointer to the next free position in buf. */
    uint8_t idx;
} term_data_t;


static term_data_t term_data =
{
    .buf = { '\0' },
    .idx = 0,
};


static void term_handle_char(term_desc_t const * const term_desc, char c)
{
    /* Depending on the connected terminal, we receive CR or LF, so we
     * handle both as end-of-command. */
    if((c == '\r') || (c == '\n'))
    {
        term_desc->put_char('\r');
        term_desc->put_char('\n');

        /* process the entered line */
        term_desc->line_handler(term_data.buf);
        
        /* reset request buffer */
        term_data.idx = 0;
        term_data.buf[0] = '\0';
    }
    else if(c == '\b')
    {
        if(term_data.idx > 0)
        {
            --term_data.idx;
            term_data.buf[term_data.idx] = '\0';

            /* Backspace is non-desctructive on VT102 (?), so a \b moves the
             * cursor back one character, but does not delete it on
             * screen. Workaround: go back one character, print a space
             * charater, print a backspace again. */
            term_desc->put_char('\b');
            term_desc->put_char(' ');
            term_desc->put_char('\b');
        }
    }
    else
    {
        if(term_data.idx >= TERM_BUF_LEN - 2)
        {
            /* overflow */
            //term_data.err = diag_err_input_length;
        }
        else
        {
            /* Add received character to buffer and always terminate with \0. */
            term_data.buf[term_data.idx] = (char)c;
            term_data.idx++;
            term_data.buf[term_data.idx] = '\0';

            term_desc->put_char(c);
        }
    }
}


/**
 * Main method of terminal implementation.
 *
 * @param term_desc The terminal descriptor providing low-level functions to
 * read and write characters to the terminal.
 */
void term_main(term_desc_t const * const term_desc)
{
    int c;
    
    while(true)
    {
        c = term_desc->get_char();
        if(c < 0)
        {
            break;
        }

        term_handle_char(term_desc, (char)c);
    }
}
