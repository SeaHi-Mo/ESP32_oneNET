/**
 * @file initialToken.c
 * @author your name (you@domain.com)
 * @brief   自动计算需要连接的信息：ProductId、DeviceName、Token
 * @version 0.1
 * @date 2022-02-09
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "mbedtls/base64.h"
#include "hmac.h"

#include "initialToken.h"

 /**
  * @brief 连接信息
  *
  */
typedef struct {
    char* et;
    char* version;
    char* method;
    char res[128];
    char sign[128];
} CONNECT_MSG;
/**
 * @brief URL编码参数 结构体
 *
 */
typedef  struct {
    char* old_str;
    char* str;
}URL_PARAMETES;
/**
 * @brief 字符串替换函数
 *
 * @param cp
 * @param n
 * @param str
 */
static void str_replace(char* cp, char* str)
{
    int lenofstr;
    char* tmp;
    lenofstr = strlen(str);
    tmp = cp;
    while (*tmp) tmp++;
    while (tmp>=cp+1)
    {
        *(tmp+(lenofstr-1)) = *tmp;
        tmp--;
    }
    strncpy(cp, str, lenofstr);
}

/**
 * @brief url_encoding_for_token
 *        连接信息进行URL 编码输出token
 * @param msg
 * @return char*
 */
char* url_encoding_for_token(CONNECT_MSG* msg)
{

    CONNECT_MSG* temp_msg = msg;
    URL_PARAMETES url_patametes[] = {
       {"+","%2B"},
       {" ","%20"},
       {"/","%2F"},
       {"?","%3F"},
       {"%%","%25"},
       {"#","%23"},
       {"&","%26"},
       {"=","%3D"},
    };
    char* token = calloc(1, 512);
    char* p = malloc(128);
    /**
     * @brief res 进行url编码
     *
     */
    memset(p, 0, 128);
    for (size_t j = 0;j<8;j++) {
        p = strstr(temp_msg->res, url_patametes[j].old_str);
        while (p) {
            str_replace(p, url_patametes[j].str);
            p += strlen(url_patametes[j].str);
            p = strstr(p, url_patametes[j].old_str);
        }
    }
    free(p);
    /**
     * @brief sign 进行url 编码
     *
     */
    p = malloc(128);
    memset(p, 0, 128);
    for (size_t j = 0;j<8;j++) {
        p = strstr(temp_msg->sign, url_patametes[j].old_str);
        while (p) {
            str_replace(p, url_patametes[j].str);
            p += strlen(url_patametes[j].str);
            p = strstr(p, url_patametes[j].old_str);
        }
    }
    free(p);
    sprintf(token, "version=%s&res=%s&et=%s&method=%s&sign=%s", temp_msg->version, temp_msg->res, temp_msg->et, temp_msg->method, temp_msg->sign);
    return token;
}
/**
 * @brief onenet_connect_msg_init
 *        连接信息初始化,计算token 并且输出 oneNET_connect_msg：ProductId、DeviceName、Token
 * @param oneNET_connect_msg 存放 连接信息的结构体
 * @param token_method 加密模式： 
 * @param  ONENET_METHOD_MD5：hmac_md5
 * @param  ONENET_METHOD_SHA1:hmac_sha1
 * @param  ONENET_METHOD_SHA256:hmac_sha256
 * @return esp_err_t
 */
esp_err_t onenet_connect_msg_init(oneNET_connect_msg_t* oneNET_connect_msg, method_t token_method)
{
    char plaintext[64] = { 0 };
    char ciphertext[64] = { 0 };
    char hmac[64] = { 0 };

    strcpy(oneNET_connect_msg->produt_id, ONENET_PRODUCT_ID);

    CONNECT_MSG oneNET_msg = {
         .et = "1959846627",
         .version = "2018-10-31",
    };
    uint8_t* mac = calloc(1, 6);
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    sprintf(oneNET_connect_msg->device_name, "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    memset(oneNET_msg.res, 0, sizeof(oneNET_msg.res));

    sprintf(oneNET_msg.res, "products/%s/devices/%s", oneNET_connect_msg->produt_id, oneNET_connect_msg->device_name);

    size_t declen = 0, enclen = 0;
#ifdef ONENET_DEVICE_DYNAMIC_ENABLE 

    memset(plaintext, 0, 64);
    mbedtls_base64_decode(NULL, enclen, &declen, (unsigned char*)ONENET_PRODUCT_KEY, enclen);
    mbedtls_base64_decode((unsigned char*)plaintext, sizeof(plaintext), &declen, (unsigned char*)ONENET_PRODUCT_KEY, strlen((char*)ONENET_PRODUCT_KEY));

#else
    mbedtls_base64_decode(NULL, enclen, &declen, (unsigned char*)ONENET_DEVICE_KEY, enclen);
    mbedtls_base64_decode((unsigned char*)plaintext, sizeof(plaintext), &declen, (unsigned char*)ONENET_DEVICE_KEY, strlen((char*)ONENET_DEVICE_KEY));
#endif
    /**
     * @brief 加密生成 hmac 密文
     *
     */
    char StringForSignature[512] = { 0 };

    switch (token_method)
    {
        case ONENET_METHOD_MD5:
            oneNET_msg.method = "md5";
            sprintf(StringForSignature, "%s\n%s\n%s\n%s", oneNET_msg.et, oneNET_msg.method, oneNET_msg.res, oneNET_msg.version);
            esp_hmac_md5((unsigned char*)plaintext, declen, (unsigned char*)StringForSignature, strlen(StringForSignature), (unsigned char*)hmac);
            break;
        case ONENET_METHOD_SHA1:
            oneNET_msg.method = "sha1";
            sprintf(StringForSignature, "%s\n%s\n%s\n%s", oneNET_msg.et, oneNET_msg.method, oneNET_msg.res, oneNET_msg.version);
            esp_hmac_sha1((unsigned char*)plaintext, declen, (unsigned char*)StringForSignature, strlen(StringForSignature), (unsigned char*)hmac);
            break;
        case ONENET_METHOD_SHA256:
            oneNET_msg.method = "sha256";
            sprintf(StringForSignature, "%s\n%s\n%s\n%s", oneNET_msg.et, oneNET_msg.method, oneNET_msg.res, oneNET_msg.version);
            esp_hmac_sha256((unsigned char*)plaintext, declen, (unsigned char*)StringForSignature, strlen(StringForSignature), (unsigned char*)hmac);
            break;
    }
    /**
     * @brief base64 编码
     *
     */
    mbedtls_base64_encode((unsigned char*)ciphertext, sizeof(ciphertext), &enclen, (unsigned char*)hmac, strlen(hmac));
    memset(oneNET_msg.sign, 0, sizeof(oneNET_msg.sign));
    strcpy(oneNET_msg.sign, ciphertext);
    //  ESP_LOG_BUFFER_HEXDUMP(TAG, oneNET_msg.sign, strlen(oneNET_msg.sign), ESP_LOG_INFO);
    char* token = url_encoding_for_token(&oneNET_msg);
    strcpy(oneNET_connect_msg->token, token);
    free(mac);
    free(token);
    ESP_LOGI(TAG, "%s", oneNET_connect_msg->token);
    return ESP_OK;
}