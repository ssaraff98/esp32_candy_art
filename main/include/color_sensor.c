/*************************************************************************************************
 * Programmer:      Shristi Saraff
 * Color Sensor:    TCS34725
 * Datasheet:       https://pdf1.alldatasheet.com/datasheet-pdf/view/894928/AMSCO/TCS34725.html
 *************************************************************************************************/

#include "color_sensor.h"

#define CHECK(x) do { \
	esp_err_t ret = (x); \
	if (ret != ESP_OK) { \
		printf("R/W Error"); \
		return ret; \
	} \
} while (0)

/***********************************************
 * Initializing esp as master in master mode
 ***********************************************/
esp_err_t i2c_master_init(i2c_port_t i2c_num) {
	i2c_config_t conf;

	// Configuration parameters of I2C controller in master mode
	conf.mode = I2C_MODE_MASTER;
	conf.sda_io_num = SDA_PIN;
	conf.scl_io_num = SCL_PIN;
	conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
	conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
	conf.master.clk_speed = CLK_SPEED;

	i2c_param_config(i2c_num, &conf);									// Configuring I2C controller
	return i2c_driver_install(i2c_num, I2C_MODE_MASTER, 0, 0, 0);		// Installing I2C driver
}

/************************************
 * 
 ************************************/
esp_err_t _i2c_master_read_slave_register(i2c_port_t i2c_num, uint8_t i2c_register, uint8_t *data_read, size_t size) {
	if (size == 0) {
		return ESP_OK;
	}

	i2c_cmd_handle_t cmd_link = i2c_cmd_link_create();					// Creating command link between master and slave
	i2c_master_start(cmd_link);											// Sending start signal on the I2C bus

	// Writing the 7-bit slave address to queue with the last bit set to write
	i2c_master_write_byte(cmd_link, (TCS34725_ADDRESS << 1) | I2C_MASTER_WRITE, ACK_CHECK_ENABLE);
	i2c_master_write_byte(cmd_link, COMMAND_BIT | i2c_register, ACK_CHECK_ENABLE);	// Sending desired register to be written

	i2c_master_start(cmd_link);											// Sending repeated start signal on the I2C bus

	// Writing the 7-bit slave address to queue with the last bit set to read
	i2c_master_write_byte(cmd_link, (TCS34725_ADDRESS << 1) | I2C_MASTER_READ, ACK_CHECK_ENABLE);

	// Reading data from sensor and sending NACK value with final byte instead of ACK value
	if(size > 1) {
		i2c_master_read(cmd_link, data_read, size - 1, ACK_VALUE);
	}
	i2c_master_read_byte(cmd_link, data_read + size - 1, NACK_VALUE);

	i2c_master_stop(cmd_link);
	esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd_link, I2C_TIMEOUT_MS / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd_link);

	return ret;
}

/************************************
 * 
 ************************************/
// !!!!! Check all write function calls - if error then pass data_write by address and not value !!!!!
esp_err_t _i2c_master_write_slave_register(i2c_port_t i2c_num, uint8_t i2c_register, uint8_t data_write, size_t size) {
	i2c_cmd_handle_t cmd_link = i2c_cmd_link_create();					// Creating command link between master and slave
	i2c_master_start(cmd_link);											// Sending start signal on the I2C bus

	// Writing the 7-bit slave address to queue with the last bit set to write
	i2c_master_write_byte(cmd_link, (TCS34725_ADDRESS << 1) | I2C_MASTER_WRITE, ACK_CHECK_ENABLE);
	i2c_master_write_byte(cmd_link, COMMAND_BIT | i2c_register, ACK_CHECK_ENABLE);	// Sending desired register to be written
	i2c_master_write(cmd_link, &data_write, size, ACK_CHECK_ENABLE);		// Writing data to sensor

	i2c_master_stop(cmd_link);
	esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd_link, I2C_TIMEOUT_MS / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd_link);
	return ret;
}

/************************************
 * Enabling TCS34725 sensor
 ************************************/
esp_err_t tcs34725_enable(i2c_port_t i2c_num) {
	CHECK(_i2c_master_write_slave_register(i2c_num, ENABLE_ADDRESS, PON_ENABLE, 1));
	vTaskDelay(3);		// !!!!! Might change delay time !!!!!

	CHECK(_i2c_master_write_slave_register(i2c_num, ENABLE_ADDRESS, PON_ENABLE | AEN_ENABLE, 1));
	vTaskDelay(3);		// !!!!! Might not need delay here !!!!!

	return ESP_OK;
}

/************************************
 * Disabling TCS34725 sensor
 ************************************/
esp_err_t tcs34725_disable(i2c_port_t i2c_num) {
	uint8_t register_status = 0;

	CHECK(_i2c_master_read_slave_register(i2c_num, ENABLE_ADDRESS, &register_status, 1));
	CHECK(_i2c_master_write_slave_register(i2c_num, ENABLE_ADDRESS, register_status & ~(PON_ENABLE | AEN_ENABLE), 1));

	return ESP_OK;
}

/************************************
 * 
 ************************************/
esp_err_t i2c_tcs34725_init(i2c_port_t i2c_num, tcs34725_t *sensor, tcs34725_integration_time_t integration_time, tcs34725_gain_t gain) {
	// Checking if master is connected to slave
	uint8_t id;
	CHECK(_i2c_master_read_slave_register(i2c_num, ID_ADDRESS, &id, 1));
	if (id != TCS34725_ID) {
		printf("Sensor not connected");
		return ESP_FAIL;
	}

	// Initializing slave parameters
	sensor->initialized = true;

	// Setting slave integration time
	CHECK(_i2c_master_write_slave_register(i2c_num, ATIME_ADDRESS, integration_time, 1));
	sensor->time = integration_time;

	// Setting slave gain
	CHECK(_i2c_master_write_slave_register(i2c_num, CONTROL_ADDRESS, gain, 1));
	sensor->gain = gain;

	// Enabling device
	CHECK(tcs34725_enable(i2c_num));

	return ESP_OK;
}

/***************************************************************************
 * Combining the lower and higher bytes of color in little endian format
 ***************************************************************************/
uint16_t _convert_from_uint8_to_uint16(uint8_t low_byte, uint8_t high_byte) {
	u_int16_t color = 0x0000;

	color = low_byte;
	color = color << 8;
	color |= high_byte;

	return color;
}

/************************************
 * 
 ************************************/
void _check_normalized_color(float red, float green, float blue) {
	// Checking if skittle is red
	printf("Red: %f, Green: %f, Blue: %f\n", red, green, blue);
}

/************************************
 * 
 ************************************/
esp_err_t i2c_tcs34725_get_rgbc_data(i2c_port_t i2c_num, tcs34725_t *sensor) {
	// Checking if TCS34725 has been initialized
	if (!(sensor->initialized)) {
		return ESP_FAIL;
	}

	uint8_t red_l, red_h, green_l, green_h, blue_l, blue_h, clear_l, clear_h;

	// Getting low and high bits of red
	CHECK(_i2c_master_read_slave_register(i2c_num, R_DATA_L, &red_l, 1));
	CHECK(_i2c_master_read_slave_register(i2c_num, R_DATA_H, &red_h, 1));

	// Getting low and high bits of green
	CHECK(_i2c_master_read_slave_register(i2c_num, G_DATA_L, &green_l, 1));
	CHECK(_i2c_master_read_slave_register(i2c_num, G_DATA_H, &green_h, 1));

	// Getting low and high bits of blue
	CHECK(_i2c_master_read_slave_register(i2c_num, B_DATA_L, &blue_l, 1));
	CHECK(_i2c_master_read_slave_register(i2c_num, B_DATA_H, &blue_h, 1));

	// Getting low and high bits of clear
	CHECK(_i2c_master_read_slave_register(i2c_num, C_DATA_L, &clear_l, 1));
	CHECK(_i2c_master_read_slave_register(i2c_num, C_DATA_H, &clear_h, 1));

	vTaskDelay(3);		// !!!!! Might not need delay here !!!!!

	// Combining lower and higher bytes to uint16 in little endian format
	uint16_t red, green, blue, clear;

	red = _convert_from_uint8_to_uint16(red_l, red_h);
	blue = _convert_from_uint8_to_uint16(blue_l, blue_h);
	green = _convert_from_uint8_to_uint16(green_l, green_h);
	clear = _convert_from_uint8_to_uint16(clear_l, clear_h);

	// Normalizing RGBC colors to 0-255
	uint32_t sum = clear;

	if (clear == 0) {
		printf("Color detected is black\n");
		return ESP_OK;
	}

	float norm_red = (float)red / sum * 255.0;
	float norm_green = (float)green / sum * 255.0;
	float norm_blue = (float)blue / sum * 255.0;
	
	_check_normalized_color(norm_red, norm_green, norm_blue);
	return ESP_OK;
}