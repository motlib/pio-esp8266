#include "diag.h"

#include <Arduino.h>

void diag_main()
{
    static String data("");
    
    while(Serial.available() > 0)
    {
        int c = Serial.read();
        //Serial.println("Received: ");
        //Serial.println(c);
        
        if((c == '\r') || (c == '\n'))
        {
            // process
            Serial.print(F("Received diag command: "));
            Serial.println(data);

            int i = data.indexOf('=');
            if(i < 0)
            {
                Serial.println("ERROR: Invalid format.");
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
                    Serial.println(F("ERROR: Unknown key."));
                }
            }
            
            data = "";
        }
        else
        {
            data += (char)c;
        }
    }   
}
