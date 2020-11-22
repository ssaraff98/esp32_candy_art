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

    // Setting slave steps per revolution
    stepper_motor->num_steps = num_steps;

    return ESP_OK;
}

void drv8825_rotate(drv8825_t *stepper_motor) {
    // Set rotation direction
    ESP_ERROR_CHECK(gpio_set_level(DIR_PIN, stepper_motor->direction));

    for (int i = 0; i < stepper_motor->num_steps; i++) {
        // printf("Step number: %d\n", i);

        ESP_ERROR_CHECK(gpio_set_level(STEP_PIN, HIGH));
        vTaskDelay(STEP_LOW_DELAY); 
        ESP_ERROR_CHECK(gpio_set_level(STEP_PIN, LOW));

        if(i % STEP_MULTIPLE == 0) {
            vTaskDelay(STEP_HIGH_DELAY);                    // May have to change this
        }
        vTaskDelay(STEP_LOW_DELAY);
    }
    vTaskDelay(STEP_LOW_DELAY);
}