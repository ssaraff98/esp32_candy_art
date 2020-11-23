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

#define TIMER                   LEDC_TIMER_1            // LEDC timer
#define CHANNEL                 LEDC_CHANNEL_0          // LEDC channel
#define SPEED_MODE              LEDC_LOW_SPEED_MODE     // LEDC speed mode
#define BITS                    LEDC_TIMER_13_BIT       // Number of bits for duty resolution

#define MIN_ANGLE               0                       // Minimum servo angle in degrees
#define MAX_ANGLE               180                     // Maximum servo angle in degrees

#define MIN_PULSEWIDTH          500                     // Minimum pulse width in microseconds for SG90
#define MAX_PULSEWIDTH          2400                    // Maximum pulse width in microseconds for SG90

#define FREQUENCY               50                      // Frequency in Hz
#define PULSE_CYCLE             20000                   // Pulse cycle of SG90 in microseconds

#define DIRECTION               1                       // Direction of 1 = up, Direction of -1 = down

// Pulsewidth in uS = (angle * (MAX_PULSEWIDTH - MIN_PULSEWIDTH) / (MAX_ANGLE - MIN_ANGLE)) + MIN_PULSEWIDTH
#define C0                      500                      // Column 0 at angle 0 degrees has pulsewidth 500 uS
#define C1                      735                      // Column 1 at angle 22.5 degrees has pulsewidth 737.5 uS
#define C2                      922                      // Column 2 at angle 45 degrees has pulsewidth 975 uS
#define C3                      1150                     // Column 3 at angle 67.5 degrees has pulsewidth 1212.5 uS
#define C4                      1390                     // Column 4 at angle 90 degrees has pulsewidth 1450 uS
#define C5                      1620                     // Column 5 at angle 112.5 degrees has pulsewidth 1687.5 uS !!!!!!! CHANGED FROM 1600
#define C6                      1895                     // Column 6 at angle 135 degrees has pulsewidth 1925 uS
#define C7                      2164                   // Column 7 at angle 157.5 degrees has pulsewidth 2162.5 uS
#define C8                      2400                     // Column 8 at angle 180 degrees has pulsewidth 2400 uS

/************************
* FUNCTION DEFINITIONS
*************************/
double _calculate_duty_percentage(double percent_duty);
esp_err_t sg90_ledc_timer_init();
esp_err_t sg90_ledc_channel_init();
void sg90_calculate_duty(double pulse_width);
void sg90_position0();
void sg90_stop();

#endif