/*************************************************************************************************
 * Programmer:      Shristi Saraff
 * Motor Driver:    DRV8825
 * Motor:           17HS4401
 * Datasheet:       https://www.ti.com/lit/ds/symlink/drv8825.pdf
 *************************************************************************************************/

#include "stepper_motor.h"

/*************************************** FUNCTIONS ***************************************/

/**********************************
 * Initializing DRV8825 as slave 
 **********************************/
esp_err_t drv8825_gpio_init() {
    gpio_config_t io_conf;

    // Configuration parameters of GPIO controller pins in output mode
	io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = ((1ULL << DIR_PIN) | (1ULL << STEP_PIN));
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.pull_up_en = 0;
    io_conf.pull_down_en = 0;

    return gpio_config(&io_conf);
}

esp_err_t drv8825_init(drv8825_t *stepper_motor, int direction, int num_steps) {
    // Initializing slave parameters
	stepper_motor->initialized = true;

	// Setting slave direction
    stepper_motor->direction = direction;

    // Setting slave steps
    stepper_motor->num_steps = num_steps;

    int count = 0;

    while (count != num_steps) {
        printf("Count: %d\n", count++);
        ESP_ERROR_CHECK(gpio_set_level(DIR_PIN, count % 2));
        ESP_ERROR_CHECK(gpio_set_level(STEP_PIN, count % 2));
        vTaskDelay(100);
    }

    return ESP_OK;

}