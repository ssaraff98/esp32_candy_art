#include "esp_err.h"
#include "include/color_sensor.h"
#include "sdkconfig.h"

/***************************************
 * Testing color sensor functionality
 ***************************************/
void app_main(void) {
	i2c_master_init(I2C_PORT_NUM);

	tcs34725_t sensor;
	tcs34725_integration_time_t integration_time = integration_time_2_4ms;
	tcs34725_gain_t gain = gain_1x;
	esp_err_t ret = i2c_tcs34725_init(I2C_PORT_NUM, &sensor, integration_time, gain);
	if (ret != ESP_OK) {
		printf("Initialization error");
		return;
	}

	ret = i2c_tcs34725_get_rgbc_data(I2C_PORT_NUM, &sensor);
    if (ret != ESP_OK) {
		printf("Get RGB data error");
		return;
	}
}