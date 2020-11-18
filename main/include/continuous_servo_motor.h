/*************************************************************************************************
 * Programmer:      Shristi Saraff
 * Motor:           FS90R
 * Datasheet:       https://www.addicore.com/FS90R-Servo-p/ad314.htm
 *************************************************************************************************/

#ifndef CONTINUOUS_SERVO_MOTOR_H
#define CONTINUOUS_SERVO_MOTOR_H

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
#define SERVO_PIN               17                      // Servo signal line GPIO pin

#define TIMER                   LEDC_TIMER_1            // LEDC timer
#define CHANNEL                 LEDC_CHANNEL_0          // LEDC channel
#define SPEED_MODE              LEDC_LOW_SPEED_MODE     // LEDC speed mode
#define BITS                    LEDC_TIMER_13_BIT       // Number of bits for duty resolution

#define MIN_ANGLE               0                       // Minimum servo angle in degrees
#define MAX_ANGLE               180                     // Maximum servo angle in degrees

#define MIN_PULSEWIDTH          700                     // Minimum pulse width in microseconds for SG90
#define MAX_PULSEWIDTH          2300                    // Maximum pulse width in microseconds for SG90

#define FREQUENCY               50                      // Frequency in Hz
#define PULSE_CYCLE             20000                   // Pulse cycle of SG90 in microseconds
#define STOP                    1500                    // Pulse in microseconds to stop the servo

#define DIRECTION               1                       // Direction of 1 = up, Direction of -1 = down

/************************
* FUNCTION DEFINITIONS
*************************/

#endif