/**
 * @file
 *
 * Implementation of a simple terminal / command-line interface. 
 */

#include "term.h"
#include "term_cfg.h"

#include <stdint.h>



/* Key definitiosn */

#define TERM_KEY_CR '\r'
#define TERM_KEY_LF '\n'
#define TERM_KEY_BACKSPACE '\b'
#define TERM_KEY_CTRL_C '\03'


/**
 * Print a string to the terminal.
 *
 * @param term_desc The terminal descriptor data structure.
 * @param str The string to print.
 */
void term_put_str(term_desc_t const * const term_desc, char const * str)
{
    while(*str)
    {
        term_desc->put_char(*str);

        str++;
    }
}



static void term_handle_char(term_desc_t * const term_desc, char c)
{
    /* Depending on the connected terminal, we receive CR or LF, so we
     * handle both as end-of-command. */
    if((c == TERM_KEY_CR) || (c == TERM_KEY_LF))
    {
        term_put_str(term_desc, TERM_LINEFEED);

        /* process the entered line */
        term_desc->line_handler(term_desc);

        /* reset request buffer */
        term_desc->idx = 0;
        term_desc->buf[0] = '\0';

        if(term_desc->flags & TERM_FLAG_PROMPT)
        {
            term_put_str(term_desc, TERM_PROMPT);
        }
    }
    else if(c == TERM_KEY_BACKSPACE)
    {
        if(term_desc->idx > 0)
        {
            --term_desc->idx;
            term_desc->buf[term_desc->idx] = '\0';

            /* Backspace is non-desctructive on VT102 (?), so a \b moves the
             * cursor back one character, but does not delete it on
             * screen. Workaround: go back one character, print a space
             * charater, print a backspace again. */
            term_desc->put_char('\b');
            term_desc->put_char(' ');
            term_desc->put_char('\b');
        }
    }
    else if(c == TERM_KEY_CTRL_C)
    {
        /* reset request buffer without handling contents. */
        term_desc->idx = 0;
        term_desc->buf[0] = '\0';

        term_put_str(term_desc, TERM_LINEFEED);
        term_put_str(term_desc, TERM_PROMPT);
    }
    else
    {
        if(term_desc->idx >= term_desc->buf_len - 2)
        {
            /* overflow */
        }
        else
        {
            /* Add received character to buffer and always terminate with \0. */
            term_desc->buf[term_desc->idx] = (char)c;
            term_desc->idx++;
            term_desc->buf[term_desc->idx] = '\0';

            if(term_desc->flags & TERM_FLAG_ECHO)
            {
                term_desc->put_char(c);
            }
        }
    }
}


/**
 * Init the terminal.
 *
 * This clears the screen by sending the VT102 reset code.
 */
void term_init(term_desc_t * const term_desc)
{
    term_put_str(term_desc, TERM_VT102_RESET);
    term_put_str(term_desc, TERM_PROMPT);
}


/**
 * Main method of terminal implementation.
 *
 * @param term_desc The terminal descriptor providing low-level functions to
 *   read and write characters to the terminal.
 */
void term_main(term_desc_t * const term_desc)
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
