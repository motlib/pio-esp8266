#include "mqtt.h"

#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#include "utils/sm.h"
#include "net/wifi.h"
#include "sensor.h"
#include "cfg/cfg.h"


#define MQTT_STATE_OFFLINE 0
#define MQTT_STATE_GO_ONLINE 1
#define MQTT_STATE_ONLINE 2


#define MQTT_PUB_CYCLE 100*60*5


static WiFiClient clt;
static PubSubClient mqtt(clt);

static sm_state_t mqtt_rq_state = MQTT_STATE_ONLINE;

static void mqtt_entry_offline(void)
{

}

static sm_state_t mqtt_do_offline(void)
{
    if((wifi_get_state() == WIFI_ONLINE)
       && (mqtt_rq_state == MQTT_STATE_ONLINE))
    {
        return MQTT_STATE_GO_ONLINE;
    }
    else
    {
        return MQTT_STATE_OFFLINE;
    }

}

static void mqtt_entry_go_online(void)
{
}

static sm_state_t mqtt_do_go_online(void)
{
    //TODO: generate client id
    char const * client_id = "motlibxQjO";

    if((cfg_mqtt.broker[0] != 0) && (cfg_mqtt.port != 0))
    {
        mqtt.setServer(cfg_mqtt.broker, cfg_mqtt.port);
    }
    else
    {
        return MQTT_STATE_OFFLINE;
    }

    if((cfg_mqtt.user[0] != 0) && (cfg_mqtt.password[0] != 0))
    {
        mqtt.connect(client_id, cfg_mqtt.user, cfg_mqtt.password);
    }
    else
    {
        return MQTT_STATE_OFFLINE;
    }

    if(mqtt.connected())
    {
        return MQTT_STATE_ONLINE;
    }
    else
    {
        return MQTT_STATE_GO_ONLINE;
    }
}

static void mqtt_entry_online(void)
{

}


static sm_state_t mqtt_do_online(void)
{
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
    SM_TBL_ENTRY(mqtt_do_offline, mqtt_entry_offline, NULL),
    SM_TBL_ENTRY(mqtt_do_go_online, mqtt_entry_go_online, NULL),
    SM_TBL_ENTRY(mqtt_do_online, mqtt_entry_online, NULL),
};


/* Statemachine configuration */
static sm_cfg_t mqtt_sm_cfg = SM_DEF_CFG(MQTT_STATE_OFFLINE, mqtt_sm_tbl);


/* Statemachine run-time data */
static sm_data_t mqtt_sm_data = SM_DEF_DATA();


void mqtt_main(void)
{
    sm_step(&mqtt_sm_cfg, &mqtt_sm_data);

    mqtt.loop();
}


void mqtt_publish(char const * const topic, char const * const msg)
{
    if(mqtt.connected())
    {
        mqtt.publish(topic, msg);
    }
}
