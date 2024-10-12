# WiFi Provisioning Component

## 介绍

`wifi_prov` 组件是一个用于通过手机蓝牙对 ESP32 进行 WiFi 配网的组件。该组件使用 ESP-IDF 提供的 WiFi 配网管理器 (`wifi_provisioning`) 和蓝牙 (`BLE`) 方案，使得用户可以通过手机应用程序轻松配置 ESP32 的 WiFi 连接。

## 许可证
本组件使用 MIT 许可证。详细信息请查看 [LICENSE](LICENSE) 文件。

## 功能

- 通过蓝牙进行 WiFi 配网。
- 支持 WiFi 连接状态检查。
- 支持 WiFi 连接失败后的重试机制。

## 使用方法

使用 `wifi_prov` 组件的方法非常简单。你只需要在你的主程序中包含 `wifi_prov.h` 头文件，并调用 `wifi_prov_init()` 函数来初始化 WiFi 配网组件。以下是详细的使用步骤：

1. **包含头文件**：在你的主程序中包含 `wifi_prov.h` 头文件。
2. **初始化组件**：在 `app_main` 函数中调用 `wifi_prov_init()` 函数来初始化 WiFi 配网组件。

## 示例代码

以下是一个完整的示例代码，展示如何使用 `wifi_prov` 组件：

#### `main.c`

```c
#include "wifi_prov.h"

void app_main(void) {
    wifi_prov_init();
}
```


## 更新日志
- **作者**: [宁子希](https://github.com/1589326497)
- **版本**: v1.0.0
- **更新日期**: 2024-10-11
- **更新内容**: 初始版本发布，包含基本的 WiFi 配网功能。


## 依赖

- ESP-IDF 版本: v5.3.1 及以上
- 依赖组件: `wifi_provisioning`, `esp_wifi`,` esp_netif `,`esp_event`,` nvs_flash`,` wifi_provisioning`

## 注意事项

- 在使用蓝牙配网时，确保手机应用程序支持 ESP-IDF 提供的配网协议。


