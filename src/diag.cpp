#include "diag.h"

#include <Arduino.h>

#include "system.h"


static String data("");

static diag_err_t diag_set_timer(char const * key, char const * val)
{
    int time = atoi(val);
            
    if((time >= 50) && (time <= (5 * 60 * 100)))
    {
        sys_config.sens_cycle_time = time;
        
        return diag_err_ok;
    }
    else
    {
        return diag_err_value;
    }
}

static diag_err_t diag_handle_input()
{
    int i = data.indexOf('=');
    
    if(i < 0)
    {
        return diag_err_input;
    }
  
    String key = data.substring(0, i);
    String val = data.substring(i + 1);

    if(key == "time")
    {
        diag_set_timer(key.c_str(), val.c_str());
    }
    else
    {
        return diag_err_key;
    }

    return diag_err_ok;
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
            diag_err_t result = diag_handle_input();

            Serial.print(F("RESPONSE="));
            Serial.println(result);
            
            data = "";
        }
        else
        {
            data += (char)c;
        }
    }   
}
