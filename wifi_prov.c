/**
 * @file wifi_prov.c
 * @author 宁子希
 * @brief 手机蓝牙对esp32配网
 * @version 1.1.0
 * @date 2024-11-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "wifi_prov.h"
#include <stdio.h>

static const char *TAG = "WIFI_PROV";
static char prov_name[50];  // 配网名称

// WiFi 事件回调函数
void wifi_event_callback(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "WiFi started");
    } else if (event_base == WIFI_PROV_EVENT) {
        switch (event_id) {
            case WIFI_PROV_START:
                ESP_LOGI(TAG, "Provisioning started");
                break;
            case WIFI_PROV_CRED_RECV:
                wifi_sta_config_t *wifi_sta_cfg = (wifi_sta_config_t *)event_data;
                ESP_LOGI(TAG, "Received Wi-Fi credentials\n\tSSID     : %s\n\tPassword : %s",
                         (const char *)wifi_sta_cfg->ssid,
                         (const char *)wifi_sta_cfg->password);
                break;
            case WIFI_PROV_CRED_FAIL:
                ESP_LOGI(TAG, "Provisioning failed");
                break;
            case WIFI_PROV_CRED_SUCCESS:
                ESP_LOGI(TAG, "Provisioning success");
                break;
            case WIFI_PROV_END:
                ESP_LOGI(TAG, "Provisioning ended");
                wifi_prov_mgr_deinit(); // 释放资源
                break;
            default:
                break;
        }
    } else if (event_base == IP_EVENT) {
        // 处理 IP 事件
    }
}

// 尝试获取当前连接的 AP 信息
esp_err_t wifi_prov_check_connection(void) {
    wifi_ap_record_t ap_info;
    esp_err_t ret = esp_wifi_sta_get_ap_info(&ap_info);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "Connected to Wi-Fi");
        ESP_LOGI(TAG, "SSID: %s", ap_info.ssid);
        return ESP_OK; // 已连接
    } else if (ret == ESP_ERR_WIFI_NOT_STARTED) {
        ESP_LOGE(TAG, "Wi-Fi not started");
    } else {
        ESP_LOGE(TAG, "Not connected to Wi-Fi, error: %s", esp_err_to_name(ret));
    }

    return ret; // 返回错误码
}

// 初始化 WiFi 配网组件
void wifi_prov_start(const char* ble_name) {

    sprintf(prov_name, "%s%s", "PROV_", ble_name);
    nvs_flash_init();
    esp_netif_init();   // 初始化 lwIP 框架
    esp_event_loop_create_default();  // 创建事件循环
    esp_netif_create_default_wifi_sta();    // 创建默认的 WiFi station
    wifi_init_config_t wifi_init_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_init_config);    // 初始化 WiFi
    
    bool provisioned = false; // 判断是否已经配网
    ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));

    // 注册事件
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_callback, NULL);
    esp_event_handler_register(WIFI_PROV_EVENT, ESP_EVENT_ANY_ID, wifi_event_callback, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_callback, NULL);

    wifi_prov_mgr_config_t prov_config = {
        .scheme = wifi_prov_scheme_ble,
        .scheme_event_handler = WIFI_PROV_SCHEME_BLE_EVENT_HANDLER_FREE_BLE,
    };
    // 初始化配网管理器
    wifi_prov_mgr_init(prov_config);

    int retry_count = 0; // 连接重试计数

    if (!provisioned) {
        wifi_prov_mgr_start_provisioning(WIFI_PROV_SECURITY_0, NULL, prov_name, NULL);
    } else {
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_start());

        ESP_LOGI(TAG, "Connecting to Wi-Fi...");

        while (retry_count < MAX_RETRIES) {
            esp_err_t send_err = esp_wifi_connect(); // 尝试发送连接
            esp_err_t connected = wifi_prov_check_connection(); // 检查是否已连接

            if (ESP_OK == connected && send_err == ESP_OK) {
                retry_count = 0;
                wifi_prov_mgr_deinit(); // 释放资源
                break; // 退出循环
            }

            ESP_LOGE(TAG, "Failed to connect to Wi-Fi, attempt %d", retry_count + 1);
            retry_count++;
            vTaskDelay(pdMS_TO_TICKS(2500)); // 等待2.5秒后再试
        }

        if (retry_count >= MAX_RETRIES) {
            ESP_LOGI(TAG, "Failed to connect after %d attempts, starting provisioning again...", MAX_RETRIES);
            retry_count = 0; // 重置计数器

            // 再次开启连接
            wifi_prov_mgr_start_provisioning(WIFI_PROV_SECURITY_0, NULL, prov_name, NULL);
        }
    }
}