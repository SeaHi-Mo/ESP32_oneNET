#ifndef APP_MQTT_CODE_H
#define APP_MQTT_CODE_H
#include "initialToken.h"
// 218.201.45.7
#define ONENET_HOST "studio-mqtt.heclouds.com"
#define ONENET_PORT 1883
#define ONENET_CLIENT_ID "001"
#define ONENET_USER_NAME "ix6VgmdyYg"
#define ONENET_PWD_TOKEN "version=2018-10-31&res=products%2Fix6VgmdyYg%2Fdevices%2F001&et=1672739519&method=md5&sign=z0Ctq2Y6U9zW0Lk0dFhZrA%3D%3D"

esp_err_t app_open_mqtt_connection(oneNET_connect_msg_t* oneNET_connect_msg);
#endif
