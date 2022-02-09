# 使用说明

## 前言

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;本源码库使用于ESP32系列及ESP8266系类连接oneNET Studio 平台，并且不依赖编译环境。

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;集成了token 计算算法，同时支持oneNET支持：

- Hmac_MD5
- Hmac_SHA1
- Hmac_SHA256

## 文件结构

```c
|-- undefined
    |-- initialToken.c  //算法源文件
    |-- initialToken.h  //算法头文件
    |-- cipher   //算法支持文件
        |-- hmac.c
        |-- hmac.h
        |-- md5.c
        |-- md5.h
        |-- sha.c
        |-- sha.h
        |-- sha1.c
        |-- sha1.h
        |-- sha256.c
        |-- sha256.h
```

## 使用方法

### 1.修改oneNET Studio 产品信息

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;使用本源码之前，请先获取**oneNET Studio** 产品信息，包括：

- 产品ID
- 产品key或者设备key
- 无需自动注册，需要获取设备名称

以上信息获取之后，在 `initialToken.h` 文件中修改相应的信息：

```c
**
 * @brief 启动自动注册功能，不适用注释掉即可
 *
 */
 //#define ONENET_DEVICE_DYNAMIC_ENABLE 
  /**
   * @brief oneNET 的产品ID
   *
   */
#define ONENET_PRODUCT_ID " "  //产品ID

#ifdef ONENET_DEVICE_DYNAMIC_ENABLE
   /**
    * @brief oneNET 的产品密钥
    *
    */
#define ONENET_PRODUCT_KEY " " //产品KEY

#else
   /**
    * @brief oneNET 的设备密钥 不启用自动注册功能请填入设备key
    *
    */
#define ONENET_DEVICE_KEY " " //设备KEY

#endif
```

### 2.调用API 获得连接信息

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;AP说明：`esp_err_t onenet_connect_msg_init(oneNET_connect_msg_t* oneNET_connect_msg, method_t token_method);`

- oneNET_connect_msg：储存计算信息的缓冲区
- token_method：算法模式，成员：
  - ONENET_METHOD_MD5
  - ONENET_METHOD_SHA1
  - ONENET_METHOD_SHA256

- 返回：ESP_OK

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;确保oneNET Studio 产品信息无误之后，在你的应用文件中引用 `initialToken.h` 头文件：

```c
#include "initialToken.h"
```

并在连接函数中定义`oneNET_connect_msg_t` 结构体变量以存放连接信息，`oneNET_connect_msg_t` 成员：

```c
typedef struct ONENET_CONNECT_MSG
{
    char produt_id[32];
    char device_name[32];
    char token[512];
}oneNET_connect_msg_t;

```

API调用示例，使用hmac_md5 算法：

```c
oneNET_connect_msg_t oneNET_connect_msg;
onenet_connect_msg_init(&oneNET_connect_msg,ONENET_METHOD_MD5);
```

## 3.测试源码地址：https://github.com/SeaHi-Mo/ESP32_oneNET
