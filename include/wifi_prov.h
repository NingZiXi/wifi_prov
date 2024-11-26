/**
 * @file wifi_prov.h
 * @author 宁子希 (1589326497@qq.com)
 * @brief 手机蓝牙对esp32配网
 * @version 1.1.0
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */
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

/**
 * @brief 开始 WiFi 配网
 * 
 * @param ble_name    蓝牙名称
 */
void wifi_prov_start(const char* ble_name);

/**
 * @brief 检查 WiFi 连接状态
 * 
 * @return esp_err_t 
 */
esp_err_t wifi_prov_check_connection(void);

#ifdef __cplusplus
}
#endif

#endif // WIFI_PROV_H