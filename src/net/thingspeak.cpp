#include "net/thingspeak.h"
#include "net/mqtt.h"
#include "sensor.h"
#include "cfg/cfg.h"
#include "utils/vfct.h"


/* for progmem */
#include <Arduino.h>
#include <stdint.h>


#define TS_TOPIC_BUF_LEN 128
#define TS_MSG_BUF_LEN 128


typedef struct
{
    vfct_t const * const vfct;
    uint8_t field_index;
} ts_field_t;

static const ts_field_t ts_fields[] =
{
    {
        .vfct = &sensor_vfct_temp,
        .field_index = 1,
    },
    {
        .vfct = &sensor_vfct_hum,
        .field_index = 2,
    },
    {
        .vfct = &sensor_vfct_pres,
        .field_index = 3,
    },

};

static const uint8_t ts_field_count = sizeof(ts_fields) / sizeof(ts_fields[0]);


/* thingspeak supports publish by one request / message per channel or per
 * field. When publishiung per field, the rate limit needs to be observed. So
 * better publish to all fields in one message. */
static void ts_publish_fields(void)
{
    static const char * topic_tmpl PROGMEM = "channels/%s/publish/%s";

    char topic[TS_TOPIC_BUF_LEN];
    char msg[TS_MSG_BUF_LEN] = {0};
    char * msg2 = msg;

    snprintf_P(topic, TS_TOPIC_BUF_LEN, topic_tmpl,
               cfg_mqtt.ts_channel,
               cfg_mqtt.ts_channel_key);


    int len = 0;

    for(uint8_t i = 0; i < ts_field_count; ++i)
    {
        /* At the moment, only float values supported. */
        len = snprintf(msg2, TS_MSG_BUF_LEN - len, "field%u=", ts_fields[i].field_index);
        msg2 += len;

        len = vfct_fmt(msg2, TS_MSG_BUF_LEN - len, ts_fields[i].vfct);
        msg2 += len;

        len = snprintf(msg2, TS_MSG_BUF_LEN - len, "&");
        msg2 += len;
    }

    /* remove trailing & character by overwriting with \0. */
    *(msg2 - 1) = '\0';

    mqtt_publish(topic, msg);
}

void ts_init(void)
{

}

void ts_main(void)
{
    static uint32_t t = 650;

    if(t > 0)
    {
        --t;
    }

    //for(uint8_t i = 0; i < ts_field_count; ++i)
    //{
    //    /* TODO: bad hack to publish one field every 50ms */
    //    if(t == (i * 5))
    //    {
    //        ts_publish_field(&(ts_fields[i]));
    //    }
    //}

    if(t == 0)
    {
        ts_publish_fields();
        t = TS_CYCLE_TIME;
    }
}
