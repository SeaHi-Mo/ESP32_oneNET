# _oneNET 连接测试使用说明_

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**在使用之前，请查看一下`componnects/oneNET/README.md` 以配置oneNET的产品连接信息。**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;在工程目录中下配置`menuconfig` ：`menuconfig`--->`Example Connection Configuration` 。配置连接的`WiFi SSID`及`WiFi password`。

```c
idf.py menuconfig
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;然后编译烧录即可

```c
idf.py flash monitor
```



