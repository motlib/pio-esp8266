#include "debug_log.h"

#include "term/term.h"
#include "term/serterm.h"
#include "net/telnet.h"

void debug_log(char const * const str)
{
    term_put_str(&serterm_desc, str);
    term_put_str(&telnet_term_desc, str);
}

