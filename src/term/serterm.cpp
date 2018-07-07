
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


term_desc_t serterm_desc =
{
    .get_char = serterm_get_char,
    .put_char = serterm_put_char,
    .line_handler = diag_handle_input,
};

