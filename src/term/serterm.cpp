
#include <Arduino.h>

#include "term.h"
#include "diag/diag.h"

/** Length of the internal line buffer. */
#define SERTERM_LINE_BUF_LEN 64


static int serterm_get_char(void)
{
    if(Serial.available() > 0)
    {
        return Serial.read();
    }
    else
    {
        return -1;
    }
}


static void serterm_put_char(char c)
{
    Serial.print(c);
}


/* Line buffer */
static char serterm_line_buf[SERTERM_LINE_BUF_LEN];


term_desc_t serterm_desc =
{
    .get_char = serterm_get_char,
    .put_char = serterm_put_char,
    .line_handler = diag_handle_input,
    .buf = serterm_line_buf,
    .buf_len = sizeof(serterm_line_buf),
    .idx = 0,
    .flags = TERM_FLAG_ECHO | TERM_FLAG_PROMPT | TERM_FLAG_HANDLE_CR,
};

