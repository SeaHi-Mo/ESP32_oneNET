#ifndef APP_MQTT_CODE_H
#define APP_MQTT_CODE_H

// 218.201.45.7
#define ONENET_HOST "studio-mqtt.heclouds.com"
#define ONENET_PORT 1883
#define ONENET_CLIENT_ID "one"
#define ONENET_USER_NAME "ix6VgmdyYg"
#define ONENET_PWD_TOKEN "version=2018-10-31&res=products%2Fix6VgmdyYg%2Fdevices%2Fone&et=2590970136&method=sha256&sign=KOLdaZk3Jv8FstZ03GU7De%2BxrRU2BzeKlEDdlhgj%2BO8%3D"


esp_err_t app_open_mqtt_connection(void);
#endif
