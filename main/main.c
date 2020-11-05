#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
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

	while (1) {
		for(double i = MIN_ANGLE; i < MAX_ANGLE; i+=22.5) {
			sg90_calculate_duty(i);
			// vTaskDelay(PULSE_CYCLE / 1000);
		}

		for(double i = MAX_ANGLE; i > MIN_ANGLE; i-=22.5) {
			sg90_calculate_duty(i);
			// vTaskDelay(PULSE_CYCLE / 1000);
		}
	}
}

/************************************
 * Testing complete functionality
 ************************************/
void app_main() {
	// xTaskCreate(&tcs34725_task, "tcs34725_task", 2048, NULL, 5, NULL);
	// xTaskCreate(&drv8825_task, "drv8825_task", 2048, NULL, 10, NULL);
	xTaskCreate(&sg90_task, "sg90_task", 2048, NULL, 5, NULL);
}