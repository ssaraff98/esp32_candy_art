/*************************************************************************************************
 * Programmer:      Shristi Saraff
 * Motor:           SG90
 * Datasheet:       https://datasheetspdf.com/pdf-file/791970/TowerPro/SG90/1
 *************************************************************************************************/

#include "servo_motor.h"

/*********************************** HELPER FUNCTIONS ***********************************/

/*********************************************
 * Calculating duty cycle by percentage
 *********************************************/
double _calculate_duty_percentage(double percent_duty) {
    if (percent_duty <= 0) {
        return 0;
    }

    if (percent_duty > 100) {
        percent_duty = 100;
    }

    double duty = (percent_duty / 100.0) * ((2 << (BITS - 1)) - 1);
    return duty;
}

/*************************************** FUNCTIONS ***************************************/

/*********************************************
 * Initializing SG90 LEDC timer as slave 
 *********************************************/
esp_err_t sg90_ledc_timer_init() {
    ledc_timer_config_t timer_conf;

    // Configuration parameters of LEDC timer pins
	timer_conf.duty_resolution = BITS;
    timer_conf.clk_cfg = LEDC_AUTO_CLK;
    timer_conf.freq_hz = FREQUENCY;
    timer_conf.speed_mode = SPEED_MODE;
    timer_conf.timer_num = TIMER;

    return ledc_timer_config(&timer_conf);
}

/*********************************************
 * Initializing SG90 LEDC channel as slave 
 *********************************************/
esp_err_t sg90_ledc_channel_init() {
    ledc_channel_config_t ledc_conf;

    // Configuration parameters of LEDC channel pins
    ledc_conf.gpio_num = SERVO_PIN;
    ledc_conf.duty = 0;
    ledc_conf.hpoint = 0;
    ledc_conf.intr_type = LEDC_INTR_DISABLE;
    ledc_conf.speed_mode = SPEED_MODE;
	ledc_conf.channel = CHANNEL;
    ledc_conf.timer_sel = TIMER;

    return ledc_channel_config(&ledc_conf);
}

/*****************************************************
 * Controlling rotation of SG90 through duty cycle
 *****************************************************/
void sg90_calculate_duty(double pulse_width) {
    double duty_bits = _calculate_duty_percentage(pulse_width * 100.0 * FREQUENCY / 1000000);

    ledc_set_duty(SPEED_MODE, CHANNEL, duty_bits);
    ledc_update_duty(SPEED_MODE, CHANNEL);
    vTaskDelay(100 / portTICK_PERIOD_MS); // 500
}

void sg90_position0() {
    sg90_calculate_duty(C8);
}

/*****************************
 * Stop rotation of SG90
 *****************************/
void sg90_stop() {
    ledc_stop(SPEED_MODE, CHANNEL, 0);
}