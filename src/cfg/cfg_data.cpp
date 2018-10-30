/**
 * @file
 * 
 * This module defines the global instance of the configuration data cfg and the
 * default data to be used if configuration data in EEPROM is not valid.
 */

#include "cfg_data.h"
#include <pgmspace.h>
#include "utils/strutils.h"
#include "utils/vfct.h"


/* Ram shadow variable for general configuration data. */
cfg_data_t cfg;

/* Ram shadow variable for wifi configuration data. */
cfg_wifi_t cfg_wifi;

/* Ram shadow variable for MQTT configuration data. */
cfg_mqtt_t cfg_mqtt;


/* Default data for configuration. */
cfg_data_t const cfg_defaults PROGMEM =
{    
    /* Read sensor cycle time in [10ms]. */
    .sens_cycle_time = 500u,

    /* Over-the-air update server. */
    .ota_host = {0},
    /* Over-the-air update port. */    
    .ota_port = 80,
    /* Over-the-air update URL path. */
    .ota_path = {0},

    /* config checksum. Value is not relevant, as it is calculated before
     * writing to EEPROM. */
    .crc16 = 0x0u,
};


/* ota host */

VFCT_FCT_GET_STR(cfg.ota_host, cfg_get_ota_host);
VFCT_FCT_SET_STR(cfg.ota_host, cfg_set_ota_host, CFG_OTA_HOST_LEN);
const vfct_t cfg_vfct_ota_host =
    VFCT_DEF(string, cfg_get_ota_host, cfg_set_ota_host);


/* ota path */

VFCT_FCT_GET_STR(cfg.ota_path, cfg_get_ota_path);
VFCT_FCT_SET_STR(cfg.ota_path, cfg_set_ota_path, CFG_OTA_PATH_LEN);
const vfct_t cfg_vfct_ota_path =
    VFCT_DEF(string, cfg_get_ota_path, cfg_set_ota_path);


/* ota port */

static uint8_t cfg_get_ota_port(uint32_t *port)
{
    *port = cfg.ota_port;

    return VFCT_ERR_OK;
}

    
static uint8_t cfg_set_ota_port(uint32_t *port)
{
    if(*port <= 0xFFFFu)
    {
        cfg.ota_port = (uint16_t)(*port);

        return VFCT_ERR_OK;
    }

    return VFCT_ERR_RANGE;
}

const vfct_t cfg_vfct_ota_port =
    VFCT_DEF(u32, cfg_get_ota_port, cfg_set_ota_port);


cfg_wifi_t const cfg_wifi_defaults PROGMEM =
{
    /* general system data */
    .node_name = {0},

    /* Network data */
    .wifi_name = {0},
    .wifi_password = {0},
    .wifi_power_on_state = 1u,

    .crc16 = 0x0u,
};    


VFCT_FCT_GET_STR(cfg_wifi.node_name,cfg_get_node_name);
VFCT_FCT_SET_STR(cfg_wifi.node_name,cfg_set_node_name,CFG_NODE_NAME_LEN);
const vfct_t cfg_vfct_node_name =
    VFCT_DEF(string, cfg_get_node_name, cfg_set_node_name);

VFCT_FCT_GET_STR(cfg_wifi.wifi_name,cfg_get_wifi_name);
VFCT_FCT_SET_STR(cfg_wifi.wifi_name,cfg_set_wifi_name,CFG_WIFI_NAME_LEN);
const vfct_t cfg_vfct_wifi_name =
    VFCT_DEF(string, cfg_get_wifi_name, cfg_set_wifi_name);

VFCT_FCT_GET_STR(cfg_wifi.wifi_password,cfg_get_wifi_password);
VFCT_FCT_SET_STR(cfg_wifi.wifi_password,cfg_set_wifi_password,CFG_WIFI_PWD_LEN);
const vfct_t cfg_vfct_wifi_password =
    VFCT_DEF(string, cfg_get_wifi_password, cfg_set_wifi_password);


cfg_mqtt_t const cfg_mqtt_defaults PROGMEM =
{
    /* MQTT broker config */
    .broker = {0},
    .port = 1883,
    .user = {0},
    .password = {0},

    .crc16 = 0x0u,
};

VFCT_FCT_GET_STR(cfg_mqtt.broker, cfg_get_mqtt_broker);
VFCT_FCT_SET_STR(cfg_mqtt.broker, cfg_set_mqtt_broker, CFG_MQTT_BROKER_LEN);
const vfct_t cfg_vfct_mqtt_broker =
    VFCT_DEF(string, cfg_get_mqtt_broker, cfg_set_mqtt_broker);


static uint8_t cfg_get_mqtt_port(uint32_t *port)
{
    *port = cfg_mqtt.port;

    return VFCT_ERR_OK;
}

    
static uint8_t cfg_set_mqtt_port(uint32_t *port)
{
    if(*port <= 0xFFFFu)
    {
        cfg_mqtt.port = (uint16_t)(*port);

        return VFCT_ERR_OK;
    }

    return VFCT_ERR_RANGE;
}

const vfct_t cfg_vfct_mqtt_port =
    VFCT_DEF(u32, cfg_get_mqtt_port, cfg_set_mqtt_port);


VFCT_FCT_GET_STR(cfg_mqtt.user, cfg_get_mqtt_user);
VFCT_FCT_SET_STR(cfg_mqtt.user, cfg_set_mqtt_user, CFG_MQTT_USER_LEN);
const vfct_t cfg_vfct_mqtt_user =
    VFCT_DEF(string, cfg_get_mqtt_user, cfg_set_mqtt_user);

VFCT_FCT_GET_STR(cfg_mqtt.password, cfg_get_mqtt_password);
VFCT_FCT_SET_STR(cfg_mqtt.password, cfg_set_mqtt_password, CFG_MQTT_PASSWORD_LEN);
const vfct_t cfg_vfct_mqtt_password =
    VFCT_DEF(string, cfg_get_mqtt_password, cfg_set_mqtt_password);


/* Table of config blocks */
cfg_block_t const cfg_block_tbl[CFG_BLOCK_COUNT] =
{
    /* wifi settings */
    {
        .shadow = &cfg_wifi,
        .defaults = &cfg_wifi_defaults,
        .size = sizeof(cfg_wifi),
    },
    
    /* general settings */
    {
        .shadow = &cfg,
        .defaults = &cfg_defaults,
        .size = sizeof(cfg),
    },
    
    /* MQTT settings */
    {
        .shadow = &cfg_mqtt,
        .defaults = &cfg_mqtt_defaults,
        .size = sizeof(cfg_mqtt),
    }
};
