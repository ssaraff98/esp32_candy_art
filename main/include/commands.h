/*************************************************************************************************
 * Programmer:      Shristi Saraff
 *************************************************************************************************/

#ifndef COMMANDS_H
#define COMMANDS_H

/************************
* INCLUDES
*************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include <freertos/queue.h>
#include "freertos/task.h"

#include "include/color_sensor.h"
#include "include/servo_motor.h"
#include "include/stepper_motor.h"
#include "sdkconfig.h"

/************************
* MACROS
*************************/

// Command ID Variables
#define C0_ID                   0
#define C1_ID                   1
#define C2_ID                   2
#define C3_ID                   3
#define C4_ID                   4
#define C5_ID                   5
#define C6_ID                   6
#define C7_ID                   7
#define C8_ID                   8
#define RESET_ID                9
#define START_ID                10
#define READ_ID                 11

extern QueueHandle_t queue;

/************************
* STRUCTS
*************************/
typedef struct command {
    int command_id;
} command_t;

/************************
* FUNCTION DEFINITIONS
*************************/
void process_commands(void *pvParameters);
void reset();

#endif