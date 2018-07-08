#include "telnet.h"

#include "term/term.h"
#include <ESP8266WiFi.h>


static WiFiServer telnet_srv(23);
static WiFiClient telnet_client;


static int telnet_get_char(void)
{
    if(telnet_client.available())
    {
        return telnet_client.read();
    }
    else
    {
        return -1;
    }
}

static void telnet_put_char(char c)
{
    if(telnet_client.connected())
    {
        telnet_client.write(c);
    }
}

void telnet_init(void)
{
    telnet_srv.begin();
}

void telnet_main(void)
{
    if(telnet_srv.hasClient())
    {
        /* simple policy: kick out the last client when a new one arrives */
        if(telnet_client.connected())
        {
            telnet_client.stop();
        }

        telnet_client = telnet_srv.available();
    }
}

static void telnet_line_handler(char * line)
{
    term_put_str(&telnet_term_desc, "yeah, got it!\r\n");
}


static char telnet_line_buf[64];

term_desc_t telnet_term_desc =
{
    .get_char = telnet_get_char,
    .put_char = telnet_put_char,
    .line_handler = telnet_line_handler,
    .buf = telnet_line_buf,
    .buf_len = sizeof(telnet_line_buf),
    .idx = 0,
};

