#include "wifi_config.h"

void wifiInit(){
	ESP_ERROR_CHECK(nvs_flash_init());
	tcpip_adapter_init();
	//Stop DHCP server
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP));
	
	//Assigning static IP
	tcpip_adapter_ip_info_t ipInfo;
	
    IP4_ADDR(&ipInfo.ip, 192, 168, 1, 1);
	IP4_ADDR(&ipInfo.gw, 192, 168, 1, 1);
	IP4_ADDR(&ipInfo.netmask, 255, 255, 255, 0);
	ESP_ERROR_CHECK(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &ipInfo));
	
	//start DHCP server
	ESP_ERROR_CHECK(tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP));
	
	printf("Server started \n");
	//ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
	ESP_ERROR_CHECK(esp_event_loop_create_default());
	
	wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
	
	ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
	
	ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
	
	wifi_config_t ap_config = {
		.ap = {
			.ssid = AP_SSID,
			.password = AP_PASSWORD,
			.max_connection = AP_MAX_CONN,
			.channel = AP_CHANNEL,
			.ssid_hidden = 0,
		},
	};
	
	ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP,&ap_config));
	
	ESP_ERROR_CHECK(esp_wifi_start());
	printf("My IP: " IPSTR "\n",IP2STR(&ipInfo.ip));

}