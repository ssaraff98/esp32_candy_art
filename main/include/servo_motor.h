/*************************************************************************************************
 * Programmer:      Shristi Saraff
 * Motor Driver:    SG90
 * Datasheet:       https://datasheetspdf.com/pdf-file/791970/TowerPro/SG90/1
 *************************************************************************************************/

#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

/************************
* INCLUDES
*************************/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

/************************
* MACROS
*************************/
// LEDC Variables
#define SERVO_PIN               16                      // Servo signal line GPIO pin

#define TIMER                   LEDC_TIMER_0            // 
#define CHANNEL                 LEDC_CHANNEL_0          // 
#define SPEED_MODE              LEDC_HIGH_SPEED_MODE    // 
#define BITS                    LEDC_TIMER_15_BIT       // 

#define MIN_ANGLE               0                       // Minimum servo angle in degrees
#define MAX_ANGLE               180                     // Maximum servo angle in degrees

#define MIN_PULSEWIDTH          500                     // Minimum pulse width in microseconds for SG90
#define MAX_PULSEWIDTH          2400                    // Maximum pulse width in microseconds for SG90

#define FREQUENCY               50                      // Frequency in Hz
#define PULSE_CYCLE             20                      // Pulse cycle of SG90 in milliseconds
#define OPERATING_SPEED         120                     // Operating speed of SG90 in milliseconds

#define DIRECTION               1                       // Direction of 1 = up, Direction of -1 = down

/************************
* FUNCTION DEFINITIONS
*************************/
double _calculate_duty_percentage(double percent_duty);
esp_err_t sg90_ledc_timer_init();
esp_err_t sg90_ledc_channel_init();
void sg90_calculate_duty(double angle);

#endif