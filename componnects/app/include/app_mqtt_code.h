#ifndef APP_MQTT_CODE_H
#define APP_MQTT_CODE_H
#include "initialToken.h"
// 218.201.45.7
#define ONENET_HOST "studio-mqtt.heclouds.com"
#define ONENET_PORT 1883
esp_err_t app_open_mqtt_connection(oneNET_connect_msg_t* oneNET_connect_msg);
#endif
