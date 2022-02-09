#ifndef INITIALTOKEN_H
#define INITIALTOKEN_H


//使能自动注册,屏蔽 则不使用自动注册功能

#define ONENET_DEVICE_DYNAMIC_ENABLE 

#define ONENET_PRODUCT_ID "ix6VgmdyYg"  //产品ID


#ifdef ONENET_DEVICE_DYNAMIC_ENABLE

#define ONENET_PRODUCT_KEY "+0fTu1ndNk3I8D3QUazGnsP024G0VQaPm8dnz+Iixm8=" //产品KEY

#elif

#define ONENET_DEVICE_KEY "" //设备KEY

#endif

typedef enum {
    ONENET_METHOD_MD5 = 0,
    ONENET_METHOD_SHA1,
    ONENET_METHOD_SHA256,
}method_t;

typedef struct ONENET_CONNECT_MSG
{
    char produt_id[32];
    char device_name[32];
    char token[512];
}oneNET_connect_msg_t;

esp_err_t onenet_connect_msg_init(oneNET_connect_msg_t* oneNET_connect_msg, method_t token_method);

#endif
