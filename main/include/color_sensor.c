/*************************************************************************************************
 * Programmer:      Shristi Saraff
 * Color Sensor:    TCS34725
 * Datasheet:       https://pdf1.alldatasheet.com/datasheet-pdf/view/894928/AMSCO/TCS34725.html
 *************************************************************************************************/

#include "color_sensor.h"

/*************************************** MACROS ****************************************/

// Error checking
#define CHECK(x) do { \
	esp_err_t ret = (x); \
	if (ret != ESP_OK) { \
		printf("R/W Error"); \
		return ret; \
	} \
} while (0)

/*********************************** HELPER FUNCTIONS ***********************************/

/*************************************************
 * Reading from specified register in TCS34725
 *************************************************/
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
	esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd_link, I2C_TIMEOUT_MS / TICK_RATE_MS);
	i2c_cmd_link_delete(cmd_link);

	return ret;
}

/*************************************************
 * Writing to specified register in TCS34725
 *************************************************/
esp_err_t _i2c_master_write_slave_register(i2c_port_t i2c_num, uint8_t i2c_register, uint8_t data_write, size_t size) {
	i2c_cmd_handle_t cmd_link = i2c_cmd_link_create();					// Creating command link between master and slave
	i2c_master_start(cmd_link);											// Sending start signal on the I2C bus

	// Writing the 7-bit slave address to queue with the last bit set to write
	i2c_master_write_byte(cmd_link, (TCS34725_ADDRESS << 1) | I2C_MASTER_WRITE, ACK_CHECK_ENABLE);
	i2c_master_write_byte(cmd_link, COMMAND_BIT | i2c_register, ACK_CHECK_ENABLE);	// Sending desired register to be written
	i2c_master_write(cmd_link, &data_write, size, ACK_CHECK_ENABLE);				// Writing data to sensor

	i2c_master_stop(cmd_link);
	esp_err_t ret = i2c_master_cmd_begin(i2c_num, cmd_link, I2C_TIMEOUT_MS / TICK_RATE_MS);
	i2c_cmd_link_delete(cmd_link);
	return ret;
}

/************************************
 * Enabling TCS34725 sensor
 ************************************/
esp_err_t _tcs34725_enable(i2c_port_t i2c_num) {
	CHECK(_i2c_master_write_slave_register(i2c_num, ENABLE_ADDRESS, PON_ENABLE, 1));
	vTaskDelay(10);

	CHECK(_i2c_master_write_slave_register(i2c_num, ENABLE_ADDRESS, PON_ENABLE | AEN_ENABLE, 1));
	vTaskDelay(10);

	return ESP_OK;
}

/************************************
 * Disabling TCS34725 sensor
 ************************************/
esp_err_t _tcs34725_disable(i2c_port_t i2c_num) {
	uint8_t register_status = 0;

	CHECK(_i2c_master_read_slave_register(i2c_num, ENABLE_ADDRESS, &register_status, 1));
	CHECK(_i2c_master_write_slave_register(i2c_num, ENABLE_ADDRESS, register_status & ~(PON_ENABLE | AEN_ENABLE), 1));

	return ESP_OK;
}

/***************************************************************************
 * Combining the lower and higher bytes of color in little endian format
 ***************************************************************************/
uint16_t _convert_from_uint8_to_uint16(uint8_t low_byte, uint8_t high_byte) {
	uint16_t color = 0x0000;

	color = low_byte;
	color = color << 8;
	color |= high_byte;

	return color;
}

/*************************************** FUNCTIONS ***************************************/

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
 * Initializing TCS34725 as slave
 ************************************/
esp_err_t i2c_tcs34725_init(i2c_port_t i2c_num, tcs34725_t *sensor, tcs34725_integration_time_t integration_time, tcs34725_gain_t gain) {
	// Checking if master is connected to slave
	uint8_t id;
	CHECK(_i2c_master_read_slave_register(i2c_num, ID_ADDRESS, &id, 1));
	if (id != TCS34725_ID) {
		printf("Sensor not connected.\n");
		return ESP_FAIL;
	}
	else {
		printf("Sensor connected.\n");
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
	CHECK(_tcs34725_enable(i2c_num));

	return ESP_OK;
}

/*************************************************
 * Getting raw 16 bit RGBC value from TCS34725
 *************************************************/
esp_err_t i2c_tcs34725_get_rgbc_data(i2c_port_t i2c_num, tcs34725_t *sensor, tcs34725_rgbc_data_t *rgbc_values) {
	// Checking if TCS34725 has been initialized
	if (!(sensor->initialized)) {
		return ESP_FAIL;
	}

	uint8_t red[2];
	uint8_t green[2];
	uint8_t blue[2];
	uint8_t clear[2];

	// Reading 16 bit RGBC values from registers
	CHECK(_i2c_master_read_slave_register(i2c_num, R_DATA_L, red, 2));
	CHECK(_i2c_master_read_slave_register(i2c_num, G_DATA_L, green, 2));
	CHECK(_i2c_master_read_slave_register(i2c_num, B_DATA_L, blue, 2));
	CHECK(_i2c_master_read_slave_register(i2c_num, C_DATA_L, clear, 2));

	vTaskDelay(10);
	
	// Converting 2 8 bit values to 1 16 bit value in little endian format
	rgbc_values->red = _convert_from_uint8_to_uint16(red[0], red[1]);
	rgbc_values->blue = _convert_from_uint8_to_uint16(blue[0], blue[1]);
	rgbc_values->green = _convert_from_uint8_to_uint16(green[0], green[1]);
	rgbc_values->clear = _convert_from_uint8_to_uint16(clear[0], clear[1]);

	return ESP_OK;
}

/*******************************************
 * Enabling/disabling TCS34725 interrupt
 *******************************************/
esp_err_t i2c_tcs34725_set_interrupt(i2c_port_t i2c_num, bool flag) {
	uint8_t value;
	CHECK(_i2c_master_read_slave_register(i2c_num, ENABLE_ADDRESS, &value, 1));

	if (flag) {
		value |= AIEN_ENABLE;
	}
	else {
		value &= ~AIEN_ENABLE;
	}

	CHECK(_i2c_master_write_slave_register(i2c_num, ENABLE_ADDRESS, value, 1));
	return ESP_OK;
}
