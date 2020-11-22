#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"

#define MAX_APs     20
#define AP_SSID     "ESP32network"
#define AP_PASSWORD "12345678"
#define AP_MAX_CONN 4
#define AP_CHANNEL  0
#define PS_MODE     WIFI_PS_MAX_MODEM

void wifiInit();
void wifi_sleep();