
#include <Arduino.h>

#include "term.h"

#include "diag/diag.h"

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


static char serterm_line_buf[64];

term_desc_t serterm_desc =
{
    .get_char = serterm_get_char,
    .put_char = serterm_put_char,
    .line_handler = diag_handle_input,
    .buf = serterm_line_buf,
    .buf_len = sizeof(serterm_line_buf),
    .idx = 0,
};

