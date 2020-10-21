#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "include/color_sensor.h"
#include "sdkconfig.h"

#define IMAGE_WIDTH   8
#define IMAGE_HEIGHT  8
#define THRESHOLD	  20

/************************************
 * 
 ************************************/
void check_rgb_color(tcs34725_rgbc_data_t *rgbc_values, char pixel_info[IMAGE_HEIGHT][IMAGE_WIDTH]) {
	// Normalizing RGBC colors to 0-255
	uint32_t max = rgbc_values->clear;

	if (rgbc_values->clear == 0) {
		printf("Black\n");
		return;
	}

	float red = (float)rgbc_values->red / max * 255.0;
	float green = (float)rgbc_values->green / max * 255.0;
	float blue = (float)rgbc_values->blue / max * 255.0;

	char color = NULL;

	// Matching color detected
	if (green == 0 && blue == 0) {
		if (red == 255) {
			printf("Purple\n");
			color = 'P';
		}
		else if (red == 170) {
			printf("Red\n");
			color = 'R';
		}
	}
	else if (blue == 0 && red > 0 && green > 0) {
		printf("Green\n");
		color = 'G';
	}
	else if (green == 51 && blue == 51) {
		printf("Orange\n");
		color = 'O';
	}
	else if (green == 42.5 && blue == 42.5) {
		printf("Yellow\n");
		color = 'Y';
	}
	else {
		// printf("Red: %f, Green: %f, Blue: %f\n", red, green, blue);
	}

	int row;
	int column;

	for (int i = IMAGE_HEIGHT - 1; i >= 0; i--) {
		row = -1;
		column = -1;

		for (int j = 0; j < IMAGE_WIDTH; j++) {
			if (color == pixel_info[i][j]) {
				row = i;
				column = j;
				pixel_info[i][j] = 'X';
			}
		}
		if (row != -1 && column != -1) {
			break;
		}
	}

	printf("Row: %d, Column: %d\n", row, column);
}

/***************************************
 * Testing color sensor functionality
 ***************************************/
void tcs34725_task(void *ignore) {
	i2c_master_init(I2C_PORT_NUM);

	tcs34725_t sensor;
	tcs34725_integration_time_t integration_time = integration_time_2_4ms;
	tcs34725_gain_t gain = gain_1x;
	esp_err_t ret = i2c_tcs34725_init(I2C_PORT_NUM, &sensor, integration_time, gain);
	if (ret != ESP_OK) {
		printf("Initialization error");
		return;
	}

	char pixel_info[IMAGE_HEIGHT][IMAGE_WIDTH] = {
		{'R', 'Y', 'O', 'R', 'O', 'Y', 'R', 'Y'}, 
		{'Y', 'O', 'Y', 'G', 'P', 'G', 'O', 'G'}, 
		{'O', 'R', 'O', 'Y', 'R', 'Y', 'P', 'Y'}, 
		{'Y', 'G', 'P', 'G', 'O', 'G', 'R', 'O'}, 
		{'O', 'Y', 'R', 'Y', 'P', 'Y', 'P', 'Y'}, 
		{'P', 'G', 'O', 'G', 'R', 'O', 'G', 'R'}, 
		{'R', 'Y', 'P', 'Y', 'P', 'Y', 'P', 'G'},
		{'O', 'G', 'R', 'O', 'G', 'R', 'G', 'O'}
	};

	while (1) {
		tcs34725_rgbc_data_t rgbc_values;
		rgbc_values.red = NULL;
		rgbc_values.green = NULL;
		rgbc_values.blue = NULL;
		rgbc_values.clear = NULL;

		//vTaskDelay(60);

		ret = i2c_tcs34725_get_rgbc_data(I2C_PORT_NUM, &sensor, &rgbc_values);
		if (ret != ESP_OK) {
			printf("Get RGB data error");
			return;
		}
		
		check_rgb_color(&rgbc_values, pixel_info);

		vTaskDelay(100);
	}

	vTaskDelete(NULL);
}

/************************************
 * Testing complete functionality
 ************************************/
void app_main() {
	xTaskCreate(&tcs34725_task, "tcs34725_task", 2048, NULL, 5, NULL);
}