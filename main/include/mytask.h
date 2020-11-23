#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "esp_system.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>
#include <ctype.h>

#include "include/color_sensor.h"
#include "include/servo_motor.h"
#include "include/stepper_motor.h"

#define IMAGE_WIDTH         8               // Pixel art image wdith
#define IMAGE_HEIGHT        8               // Pixel art image height

#define BIT_0               (1 << 0)
#define SENSE_COLOR         BIT_0           // Event group bit that is set when the color sensor should get the candy color

// Delay Variables
#define RECEIVE_BIT_DELAY   1               // Time taken in milliseconds to receive bits (should be lower than time for which candy stops under sensor)
#define MOVE_SERVO_DELAY    2000            // Time taken in microseconds to move servo to correct tube for a particular candy   !!!!!!!!!!!!!! CHANGED from 1000 from 1500
#define TO_REJECT_DELAY     1000            // Time taken in milliseconds to move servo to reject tube for incorrect color

void tcs34725_task(void *ignore);
void drv8825_task(void *ignore);
void sg90_task(void *ignore);
void test_for_one_revolution_task();
void test_task();
