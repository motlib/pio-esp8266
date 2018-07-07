#ifndef TERM_H
#define TERM_H


typedef struct
{
    int (*get_char)(void);
    void (*put_char)(char c);
    void (*line_handler)(char * line);
} term_desc_t;


void term_main(term_desc_t const * const term_desc);


#endif /* TERM_H */
