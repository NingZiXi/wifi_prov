#ifndef WIFI_PROV_H
#define WIFI_PROV_H

#include <stdbool.h>
#include "esp_err.h"
#include <nvs_flash.h>
#include "esp_netif.h"
#include "esp_wifi.h"
#include <esp_log.h>
#include "wifi_provisioning/manager.h"
#include <wifi_provisioning/scheme_ble.h>


#define MAX_RETRIES 3 // 最大重试次数

#ifdef __cplusplus
extern "C" {
#endif

// 初始化 WiFi 配网组件并运行
void wifi_prov_init(void);
// 检查当前 WiFi 连接状态
esp_err_t wifi_prov_check_connection(void);

#ifdef __cplusplus
}
#endif

#endif // WIFI_PROV_H