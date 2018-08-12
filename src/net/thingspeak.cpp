#include "net/thingspeak.h"
#include "net/mqtt.h"
#include "sensor.h"
#include "cfg/cfg.h"

/* for progmem */
#include <Arduino.h>
#include <stdint.h>


#define TS_TOPIC_BUF_LEN 128
#define TS_MSG_BUF_LEN 32


typedef struct
{
    float (*get_fct)(void);
    uint8_t field_index;
} ts_field_t;


static const ts_field_t ts_fields[] =
{
    {
        .get_fct = sensor_get_temp,
        .field_index = 1,
    },
    {
        .get_fct = sensor_get_hum,
        .field_index = 2,
    },
    {
        .get_fct = sensor_get_pres,
        .field_index = 3,
    },

};

static const uint8_t ts_field_count = sizeof(ts_fields) / sizeof(ts_fields[0]);


static void ts_publish_field(ts_field_t const * const ts_field)
{
    static const char * topic_tmpl PROGMEM = "channels/%s/publish/fields/field%u/%s";
    
    char topic[TS_TOPIC_BUF_LEN];
    char msg[TS_MSG_BUF_LEN];

    snprintf_P(topic, TS_TOPIC_BUF_LEN, topic_tmpl,
               cfg_mqtt.ts_channel,
               ts_field->field_index,
               cfg_mqtt.ts_channel_key);

    /* At the moment, only float values supported. */
    snprintf(msg, TS_MSG_BUF_LEN, "%f", ts_field->get_fct());

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

    if(t == 200)
    {
        ts_publish_field(ts_fields + 0);
    }
    if(t == 400)
    {
        ts_publish_field(ts_fields + 1);
    }
    if(t == 600)
    {
        ts_publish_field(ts_fields + 2);
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
        t = TS_CYCLE_TIME;
    }
}
