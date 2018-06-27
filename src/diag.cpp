#include "diag.h"

#include <Arduino.h>

static String data("");

static diag_err_t diag_handle_input()
{
    int i = data.indexOf('=');
    
    if(i < 0)
    {
        return diag_err_input;
    }
    else
    {
        String key = data.substring(0, i);
        String val = data.substring(i + 1);

        if(key == "time")
        {
            long t = val.toInt();
            if((t < 50) || (t > (5 * 60 * 100)))
            {
                Serial.println(F("ERROR: Invalid value."));
            }
            else
            {
                cycle_time = t;
            }
        }
        else
        {
            return diag_err_key;
        }
    }    
}

void diag_main()
{
    while(Serial.available() > 0)
    {
        int c = Serial.read();

        /* Depending on the connected terminal, we receive CR or LF, so we
         * handle both as end-of-command. */
        if((c == '\r') || (c == '\n'))
        {
            
            data = "";
        }
        else
        {
            data += (char)c;
        }
    }   
}
