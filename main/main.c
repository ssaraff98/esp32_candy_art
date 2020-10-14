#include "esp_err.h"
#include "include/color_sensor.h"
#include "sdkconfig.h"

/************************************
 * 
 ************************************/
void check_rgb_color(tcs34725_rgbc_data_t *rgbc_values) {
	// Normalizing RGBC colors to 0-255
	uint32_t max = rgbc_values->clear;
	// float max = (float)rgbc_values->red;
	// if(rgbc_values->red < rgbc_values->blue && rgbc_values->green < rgbc_values->blue)
	// 	max = (float)rgbc_values->blue;
	// else if(rgbc_values->red < rgbc_values->green && rgbc_values->blue < rgbc_values->green)
	// 	max = (float)rgbc_values->green;

	if (rgbc_values->clear == 0) {
		return;
	}

	float red = (float)rgbc_values->red / max * 255.0;
	float green = (float)rgbc_values->green / max * 255.0;
	float blue = (float)rgbc_values->blue / max * 255.0;

	// u_char red = rgbc_values->red >> 8;
	// u_char green = rgbc_values->green >> 8;
	// u_char blue = rgbc_values->blue >> 8;

	printf("Red: %f, Green: %f, Blue: %f\n", red, green, blue);
}

/***************************************
 * Testing color sensor functionality
 ***************************************/
void tcs34725_task(void *ignore) {
	i2c_master_init(I2C_PORT_NUM);

	tcs34725_t sensor;
	tcs34725_integration_time_t integration_time = integration_time_2_4ms;
	tcs34725_gain_t gain = gain_4x;
	esp_err_t ret = i2c_tcs34725_init(I2C_PORT_NUM, &sensor, integration_time, gain);
	if (ret != ESP_OK) {
		printf("Initialization error");
		return;
	}

	while (1) {
		tcs34725_rgbc_data_t rgbc_values;
		rgbc_values.red = NULL;
		rgbc_values.green = NULL;
		rgbc_values.blue = NULL;
		rgbc_values.clear = NULL;

		//i2c_tcs34725_set_interrupt(I2C_PORT_NUM, false);		// Turning LED on
		vTaskDelay(60);

		ret = i2c_tcs34725_get_rgbc_data(I2C_PORT_NUM, &sensor, &rgbc_values);
		if (ret != ESP_OK) {
			printf("Get RGB data error");
			return;
		}

		//i2c_tcs34725_set_interrupt(I2C_PORT_NUM, true);		// Turning LED off
		
		check_rgb_color(&rgbc_values);

		//vTaskDelay(60);
	}

	vTaskDelete(NULL);
}

void app_main() {
	xTaskCreate(&tcs34725_task, "tcs34725_task", 2048, NULL, 5, NULL);
}