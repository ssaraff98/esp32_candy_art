#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "include/color_sensor.h"
#include "include/servo_motor.h"
#include "include/stepper_motor.h"
#include "sdkconfig.h"

#define IMAGE_WIDTH   8
#define IMAGE_HEIGHT  8

/***************************************
 * Testing color sensor functionality
 ***************************************/
void tcs34725_task(void *ignore) {
	esp_err_t ret = i2c_master_init(I2C_PORT_NUM);
	if (ret != ESP_OK) {
		printf("I2C master initialization error");
		return;
	}

	tcs34725_t sensor;
	tcs34725_integration_time_t integration_time = integration_time_2_4ms;
	tcs34725_gain_t gain = gain_1x;
	ret = i2c_tcs34725_init(I2C_PORT_NUM, &sensor, integration_time, gain);
	if (ret != ESP_OK) {
		printf("TCS34725 initialization error");
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

	int count = 0;
	while (count < 64) {
		tcs34725_rgbc_data_t rgbc_values;
		rgbc_values.red = NULL;
		rgbc_values.green = NULL;
		rgbc_values.blue = NULL;
		rgbc_values.clear = NULL;
		int column = -1;

		ret = i2c_tcs34725_get_rgbc_data(I2C_PORT_NUM, &sensor, &rgbc_values);
		if (ret != ESP_OK) {
			printf("Get RGB data error");
			return;
		}

		printf("\n");
		
		column = check_rgb_color(&rgbc_values, pixel_info);
		if (column < 0 || column >= IMAGE_HEIGHT) {
			printf("Reject tube\n");
		}
		else {
			printf("Column: %d\n", column);
			count++;
		}
		printf("Count: %d\n", count);
		vTaskDelay(100);
	}
}

/***************************************
 * Testing stepper motor functionality
 ***************************************/
void drv8825_task(void *ignore) {
	esp_err_t ret = drv8825_gpio_init();
	if (ret != ESP_OK) {
		printf("DRV8825 GPIO initialization error");
		return;
	}

	drv8825_t stepper_motor;
	int direction = HIGH;
	int steps = STEPS_PER_REV;

	ret = drv8825_init(&stepper_motor, direction, steps);
	if (ret != ESP_OK) {
		printf("DRV8825 initialization error");
		return;
	}

	drv8825_rotate(&stepper_motor);
}

/***************************************
 * Testing servo motor functionality
 ***************************************/
void sg90_task(void *ignore) {
	esp_err_t ret = sg90_ledc_timer_init();
	if (ret != ESP_OK) {
		printf("SG90 LEDC timer initialization error");
		return;
	}

	ret = sg90_ledc_channel_init();
	if (ret != ESP_OK) {
		printf("SG90 LEDC channel initialization error");
		return;
	}

	double pulsewidth[9] = {C0, C1, C2, C3, C4, C5, C6, C7, C8};

	int column_list[IMAGE_HEIGHT * 2] = {
		0, 3, 8, 1, 1, 4, 5, 7,
		2, 2, 2, 6, 8, 0, 3, 4
	};

	vTaskDelay(1);
	sg90_position0();

	for (int c = 0; c < IMAGE_HEIGHT * 2; c++) {
		int column = column_list[c];
		printf("Pulsewidth %lf microseconds for column %d\n", pulsewidth[column], column);
		sg90_calculate_duty(pulsewidth[column]);
		vTaskDelay(100);
	}
}

/************************************
 * Modular testing functionality
 ************************************/
// void app_main() {
	// xTaskCreate(&tcs34725_task, "tcs34725_task", 2048, NULL, 5, NULL);
	// xTaskCreate(&drv8825_task, "drv8825_task", 2048, NULL, 10, NULL);
	// xTaskCreate(&sg90_task, "sg90_task", 2048, NULL, 5, NULL);
