#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "mqtt_client.h"

#include "app_mqtt_code.h"

static char* TAG = "APP_MQTT";

extern const uint8_t client_cert_pem_start[] asm("_binary_MQTTS_certificate_pem_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_MQTTS_certificate_pem_end");
/**
 * @brief mqtt 事件处理函数
 *
 * @param event
 * @return esp_err_t
 */
static esp_err_t oneNET_mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
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
