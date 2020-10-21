/*************************************************************************************************
 * Programmer:      Shristi Saraff
 * Color Sensor:    TCS34725
 * Datasheet:       https://pdf1.alldatasheet.com/datasheet-pdf/view/894928/AMSCO/TCS34725.html
 *************************************************************************************************/

#ifndef SORT_SERVO_MOTOR_H
#define SORT_SERVO_MOTOR_H

/************************
* INCLUDES
*************************/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"

/************************
* MACROS
*************************/

#define DIR_PIN                 16              // PWM direction pin on ESP32 NodeMCU
#define STEP_PIN                17              // PWM step pin on ESP32 NodeMCU
#define PWM_OUT_A1
#define PWM_OUT_A2
#define PWM_OUT_B1
#define PWM_OUT_B2

#define MIN_PULSEWIDTH          1000            // Minimum pulsewidth in microseconds
#define MAX_PULSEWIDTH          2000            // Maximum pulsewidth in microseconds
