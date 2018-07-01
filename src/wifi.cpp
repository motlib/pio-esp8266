

#define WIFI_CONNECT_TIMEOUT 1000

typedef enum
{
    WIFI_OFFLINE,
    WIFI_GO_ONLINE,
    WIFI_ONLINE,
} wifi_state_t;


typedef struct
{
    /** Current state of the wifi statemachine */
    wifi_state_t state;

    /** Request to go online. */
    uint8_t request;

    /** Wifi connect timeout. */
    uint16_t timeout;
} wifi_data_t;


static wifi_data_t wifi_data = {
    .state = WIFI_OFFLINE,
    
    .request = 0,
    
    .timeout = 0,
};


void wifi_init(void)
{
    
}
    
static void wifi_do_offline(void)
{
    if(wifi_data.request == 1)
    {
        wifi_data.timeout = WIFI_CONNECT_TIMEOUT;
        return WIFI_GO_ONLINE;
    }
    else
    {
        return WIFI_OFFLINE;
    }
}

static void wifi_do_go_online(void)
{
    //WiFi.begin(ssid, password);
    //
    //while (WiFi.status() != WL_CONNECTED) {
    //
    //    delay(1000);
    //    Serial.println("Connecting..");
    //    
    //}   
}

    
void wifi_main(void)
{
    switch(wifi_data.state)
    {
    case WIFI_OFFLINE:
        break;
    default:
        wifi_data.state = WIFI_OFFLINE;
        break;
    }
}
