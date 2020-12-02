#include "include/myserver_config.h"
#include "include/wifi_config.h"
#include "mytask.h"
#include "sdkconfig.h"

/************************************
 * Testing complete functionality
 ************************************/
void app_main() {
	//xTaskCreate(&tcs34725_task, "tcs34725_task", 2048, NULL, 5, NULL);
	// xTaskCreate(&sg90_task, "sg90_task", 2048, NULL, 5, NULL);
	//xTaskCreate(&test_for_one_revolution_task, "test_task", 2048, NULL, 1, NULL);

	spiffs_setup();
	wifiInit();
	serverconfig();
}