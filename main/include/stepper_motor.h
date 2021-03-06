/*************************************************************************************************
 * Programmer:      Shristi Saraff
 * Motor Driver:    DRV8825
 * Motor:           17HS4401
 * Datasheet:       https://www.ti.com/lit/ds/symlink/drv8825.pdf
 *************************************************************************************************/

#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

/************************
* INCLUDES
*************************/
#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include "soc/rtc.h"
#include "driver/gpio.h"

/************************
* MACROS
*************************/
// GPIO Variables
#define DIR_PIN                 11                              // PWM direction pin on ESP32 NodeMCU - 18
#define STEP_PIN                10                              // PWM step pin on ESP32 NodeMCU - 19

#define HIGH                    1                               // Rotation in clockwise direction
#define LOW                     0                               // Rotation in counter clockwise direction

// DRV8825 Variables
#define MAX_FREQUENCY           250000                          // Maximum frequency in Hz of DRV8825                   
#define STEPS_PER_REV           200                             // Tota number of steps in one revolution of the stepper

#define STEP_MULTIPLE           40                              // Approximate number of steps between two spokes of the wheel
#define STEP_LOW_DELAY          10                              // Time in microseconds between each step !!! 10
#define STEP_HIGH_DELAY         1000                            // Total time in microseconds for which the candy stops under the color sensor

/************************
* STRUCTS
*************************/
typedef struct drv8825 {
    bool initialized;
    int direction;
    int num_steps;
} drv8825_t;

/************************
* FUNCTION DEFINITIONS
*************************/
esp_err_t drv8825_gpio_init();
esp_err_t drv8825_init(drv8825_t *stepper_motor, int direction, int num_steps);
void drv8825_rotate(drv8825_t *stepper_motor);

#endif