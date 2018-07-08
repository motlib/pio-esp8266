#ifndef TELNET_H
#define TELNET_H

#include "term/term.h"

void telnet_init(void);
void telnet_main(void);

extern term_desc_t telnet_term_desc;


#endif /* TELNET_H */
