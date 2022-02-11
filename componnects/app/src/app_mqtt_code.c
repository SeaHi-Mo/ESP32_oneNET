#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "mqtt_client.h"

#include "app_mqtt_code.h"

static char* TAG = "APP_MQTT";
static oneNET_connect_msg_t* oneNET_connect_msg_static;
extern const uint8_t client_cert_pem_start[] asm("_binary_MQTTS_certificate_pem_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_MQTTS_certificate_pem_end");

/**
 * @brief oneNET_pulib_property_thread
 *          定时上传属性任务
 * @param arg
 * @return void*
 */
void* oneNET_pulib_property_thread(void* arg)
{
    esp_mqtt_client_handle_t client = arg;
    int humi = 23, temp = 25;
    char topic[128] = { 0 };
    char device_property[128] = { 0 };
    sprintf(topic, "$sys/%s/%s/thing/property/post", oneNET_connect_msg_static->produt_id, oneNET_connect_msg_static->device_name);
    while (1) {
        if (humi>100) {
            humi = 0;
            temp = 0;
        }
        humi++; temp++;
        sprintf(device_property, "{\"id\": \"123\",\"version\":\"1.0\",\"params\":{\"humi\":{\"value\":%d},\"temp\":{\"value\":%0.2f}}}", humi, (float)temp);
        ESP_LOGI(TAG, "topic:%s,data:%s", topic, device_property);
        esp_mqtt_client_publish(client, topic, device_property, strlen(device_property), 1, 0);
        sleep(10);
    }

    return NULL;
}
/**
 * @brief mqtt 事件处理函数
 *
 * @param event
 * @return esp_err_t
 */
static esp_err_t oneNET_mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;

    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            char dev_property_topic[128] = { 0 };
            //订阅 设备属性上报响应
            memset(dev_property_topic, 0, 128);
            sprintf(dev_property_topic, "$sys/%s/%s/thing/property/post/reply", oneNET_connect_msg_static->produt_id, oneNET_connect_msg_static->device_name);
            esp_mqtt_client_subscribe(client, dev_property_topic, 1);
            //设备属性设置Topic
            memset(dev_property_topic, 0, 128);
            sprintf(dev_property_topic, "$sys/%s/%s/thing/property/set", oneNET_connect_msg_static->produt_id, oneNET_connect_msg_static->device_name);
            esp_mqtt_client_subscribe(client, dev_property_topic, 1);
            //云平台主动获取属性 Topic
            memset(dev_property_topic, 0, 128);
            sprintf(dev_property_topic, "$sys/%s/%s/thing/property/get", oneNET_connect_msg_static->produt_id, oneNET_connect_msg_static->device_name);
            esp_mqtt_client_subscribe(client, dev_property_topic, 1);

            //订阅OTA opic
            memset(dev_property_topic, 0, 128);
            sprintf(dev_property_topic, "ota/%s/%s/thing/property/get", oneNET_connect_msg_static->produt_id, oneNET_connect_msg_static->device_name);
            esp_mqtt_client_subscribe(client, dev_property_topic, 1);
            //创建发布的线程
            static pthread_t oneNET_pulish_thread;
            int ret = pthread_create(&oneNET_pulish_thread, NULL, oneNET_pulib_property_thread, (void*)client);
            if (ret!=0)
                ESP_LOGE(TAG, "pthread_create fail,err=%d", ret);

            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            ret = pthread_join(oneNET_pulish_thread, NULL);
            if (ret!=0)
                ESP_LOGE(TAG, "pthread_join fail,err=%d", ret);
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);

            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}
/**
 * @brief 启动连接 oneNET
 *
 * @return esp_err_t
 */
esp_err_t app_open_mqtt_connection(oneNET_connect_msg_t* oneNET_connect_msg)
{
    onenet_connect_msg_init(oneNET_connect_msg, ONENET_METHOD_MD5);
    oneNET_connect_msg_static = oneNET_connect_msg;
    esp_mqtt_client_config_t oneNET_client_cfg = {
        .host = ONENET_HOST,
        .port = ONENET_PORT,
        .event_handle = oneNET_mqtt_event_handler,
        .client_id = oneNET_connect_msg->device_name,
        .username = oneNET_connect_msg->produt_id,
        .password = oneNET_connect_msg->token,

    };

    esp_mqtt_client_handle_t oneNET_client_handler = esp_mqtt_client_init(&oneNET_client_cfg);
    esp_mqtt_client_start(oneNET_client_handler);

    return ESP_OK;
}
