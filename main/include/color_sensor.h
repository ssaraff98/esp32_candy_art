/*************************************************************************************************
 * Programmer:      Shristi Saraff
 * Color Sensor:    TCS34725
 * Datasheet:       https://pdf1.alldatasheet.com/datasheet-pdf/view/894928/AMSCO/TCS34725.html
 *************************************************************************************************/

#ifndef COLOR_SENSOR_H
#define COLOR_SENSOR_H

/************************
* INCLUDES
*************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

/************************
* MACROS
*************************/
// I2C Variables
#define I2C_TIMEOUT_MS          1000
#define I2C_PORT_NUM            I2C_NUM_1       // Using I2C hardware port 1 on ESP32

#define ACK_VALUE               0x0
#define NACK_VALUE              0x1

#define ACK_CHECK_DISABLE       0x0
#define ACK_CHECK_ENABLE        0x1

#define SDA_PIN                 21              // I2C SDA pin on ESP32 NodeMCU
#define SCL_PIN                 22              // I2C SCL pin on ESP32 NodeMCU

#define CLK_SPEED               400000          // Fast transmission speed - 400Kbit/s

#define TICK_RATE_MS            100             // Same as portTICK_RATE_MS

// TCS34725 Variables
#define TCS34725_ADDRESS        0x29            // Slave address given in TCS34725 datasheet
#define TCS34725_ID             0x44            // ID value for TCS34725

// TCS34725 Register Address Variables
#define ATIME_ADDRESS           0x01            // RGBC timer register controls integration time
#define CONTROL_ADDRESS         0x0F            // Control register controls gain
#define ID_ADDRESS              0x12            // ID register provides value for TCS34725 (read-only)

// TCS34725 Color Register Address Variables
#define C_DATA_L                0x14            // Clear data low byte register
#define C_DATA_H                0x15            // Clear data high byte register

#define R_DATA_L                0x16            // Red data low byte register
#define R_DATA_H                0x17            // Red data high byte register

#define G_DATA_L                0x18            // Green data low byte register
#define G_DATA_H                0x19            // Green data high byte register

#define B_DATA_L                0x1A            // Blue data low byte register
#define B_DATA_H                0x1B            // Blue data high byte register

// TCS34725 Iterrupt Enable Register Variables
#define ENABLE_ADDRESS          0x00            // Interrupt enable register enables states and interrupts
#define PON_ENABLE              0x01            // Power on bit activates internal oscillators
#define AEN_ENABLE              0x02            // RGBC enable bit activates the two channel ADC
#define WEN_ENABLE              0x08            // Wait enable bit activates wait timer
#define AIEN_ENABLE             0x10            // RGBC interrupt enable bit generates RGBC interrupts

// TCS34725 Miscellaneous Variables
#define COMMAND_BIT             0x80            // Command bit specifies register address
#define RESET_REGISTERS         0x00            // Reset value for all registers except ATIME and WTIME

// Pixel Image Data Variables
#define IMAGE_WIDTH             8
#define IMAGE_HEIGHT            8
#define THRESHOLD               30

/************************
* STRUCTS
*************************/
typedef enum tcs34725_integration_time {
    integration_time_2_4ms = 0xFF,              // 2.4ms -   1 cycle  -  1024 maximum count
    integration_time_24ms  = 0xF6,              //  24ms -  10 cycles - 10240 maximum count
    integration_time_50ms  = 0xEB,              //  50ms -  20 cycles - 20480 maximum count
    integration_time_101ms = 0xD5,              // 101ms -  42 cycles - 43008 maximum count
    integration_time_154ms = 0xC0,              // 154ms -  64 cycles - 65535 maximum count
    integration_time_700ms = 0x00               // 700ms - 256 cycles - 65535 maximum count
} tcs34725_integration_time_t;

typedef enum tcs34725_gain {
    gain_1x  = 0x00,                            //  1x gain
    gain_4x  = 0x01,                            //  4x gain
    gain_16x = 0x02,                            // 16x gain
    gain_60x = 0x03                             // 60x gain
} tcs34725_gain_t;

typedef struct tcs34725 {
    bool initialized;
    tcs34725_integration_time_t time;
    tcs34725_gain_t gain;
} tcs34725_t;

typedef struct tcs34725_rgbc_data {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t clear;
} tcs34725_rgbc_data_t;

/************************
* FUNCTION DEFINITIONS
*************************/
esp_err_t _i2c_master_read_slave_register(i2c_port_t i2c_num, uint8_t i2c_register, uint8_t *data_read, size_t size);
esp_err_t _i2c_master_write_slave_register(i2c_port_t i2c_num, uint8_t i2c_register, uint8_t data_write, size_t size);
esp_err_t _tcs34725_enable(i2c_port_t i2c_num);
esp_err_t _tcs34725_disable(i2c_port_t i2c_num);
uint16_t _convert_from_uint8_to_uint16(uint8_t low_byte, uint8_t high_byte);
esp_err_t i2c_master_init(i2c_port_t i2c_num);
esp_err_t i2c_tcs34725_init(i2c_port_t i2c_num, tcs34725_t *sensor, tcs34725_integration_time_t integration_time, tcs34725_gain_t gain);
esp_err_t i2c_tcs34725_get_rgbc_data(i2c_port_t i2c_num, tcs34725_t *sensor, tcs34725_rgbc_data_t *rgbc_values);
esp_err_t i2c_tcs34725_set_interrupt(i2c_port_t i2c_num, bool flag);
int check_rgb_color(tcs34725_rgbc_data_t *rgbc_values, char pixel_info[IMAGE_HEIGHT][IMAGE_WIDTH]);

#endif