#ifndef MQTT_H
#define MQTT_H


void mqtt_init(void);
void mqtt_main(void);

void mqtt_publish(char const * const topic, char const * const msg);

#endif /* MQTT_H */
