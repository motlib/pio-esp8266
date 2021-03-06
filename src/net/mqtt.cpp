#include "mqtt.h"

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include "mqtt_cfg.h"
#include "utils/sm.h"
#include "net/wifi.h"
#include "cfg/cfg.h"


#define MQTT_STATE_OFFLINE 0
#define MQTT_STATE_GO_ONLINE 1
#define MQTT_STATE_ONLINE 2



static WiFiClient clt;
static PubSubClient mqtt(clt);

static sm_state_t mqtt_rq_state = MQTT_STATE_ONLINE;

static uint16_t mqtt_connect_timeout = 0;




/* OFFLINE state handler  */
static sm_state_t mqtt_do_offline(void)
{
    if((wifi_get_state() == WIFI_ONLINE)
       && (mqtt_rq_state == MQTT_STATE_ONLINE)
       && (cfg_mqtt.broker[0] != 0)
       && (cfg_mqtt.port != 0))
    {
        return MQTT_STATE_GO_ONLINE;
    }
    else
    {
        return MQTT_STATE_OFFLINE;
    }
}


/* Entry of GO_ONLINE state. Set up mqtt client and start connection. */
static void mqtt_entry_go_online(void)
{
    //TODO: generate client id
    char const * client_id = "motlibxQjO";

    /* Check if broker host name and port are configured. If not, stay
     * offline. */
    mqtt.setServer(cfg_mqtt.broker, cfg_mqtt.port);

    mqtt.connect(client_id, cfg_mqtt.user, cfg_mqtt.password);
        
    /* set up connect timeout */
    mqtt_connect_timeout = MQTT_CONNECT_TIMEOUT;
}

/* Wait until connection is established, then transition to 'online' state. */
static sm_state_t mqtt_do_go_online(void)
{
    /* TODO: need timeout */
    if(mqtt.connected())
    {
        return MQTT_STATE_ONLINE;
    }
    else
    {
        if(mqtt_connect_timeout != 0)
        {
            --mqtt_connect_timeout;
            
            return MQTT_STATE_GO_ONLINE;
        }
        else
        {
            /* timeout expired. Go back to offline. */
            return MQTT_STATE_OFFLINE;
        }
    }
}



/* ONLINE state handler */
static sm_state_t mqtt_do_online(void)
{
    /* Check if we lost the connection. If yes, go back to 'offline' state to
     * re-start the connection procedure. Otherwise just stay online. */
    if(mqtt.connected())
    {
        return MQTT_STATE_ONLINE;
    }
    else
    {
        return MQTT_STATE_OFFLINE;
    }
}

void mqtt_init(void)
{
    
}


/* Statemachine table for wifi handling */
static sm_tbl_entry_t mqtt_sm_tbl[] =
{
    SM_TBL_ENTRY(mqtt_do_offline, NULL, NULL),
    SM_TBL_ENTRY(mqtt_do_go_online, mqtt_entry_go_online, NULL),
    SM_TBL_ENTRY(mqtt_do_online, NULL, NULL),
};


/* Statemachine configuration */
static sm_cfg_t mqtt_sm_cfg = SM_DEF_CFG(MQTT_STATE_OFFLINE, mqtt_sm_tbl);

/* Statemachine run-time data */
static sm_data_t mqtt_sm_data = SM_DEF_DATA();


void mqtt_main(void)
{
    /* Run our own statemachine to handle mqtt connection. */
    sm_step(&mqtt_sm_cfg, &mqtt_sm_data);

    /* Process pending work of mqtt client. */
    mqtt.loop();
}


/* Publish a message. Publish is only done if we are online. */
void mqtt_publish(char const * const topic, char const * const msg)
{
    char topic_buf[128];
    
    if(mqtt.connected())
    {
        snprintf(topic_buf, 128, "%s/%s", cfg_wifi.node_name, topic);
        
        mqtt.publish(topic_buf, msg);
    }
}


