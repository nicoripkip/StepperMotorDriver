#include "motordriver.hpp"
#include "Arduino.h"


/**
 * @brief Constructor
 * 
 * @param motor
 * @param pins
 * @param gears
 * @param rpm
 * @param enable_microstepping
 * @param conf
 */
MotorDriver::MotorDriver(motor_data_t* motor, int* pins, uint8_t gears, uint8_t rpm, bool enable_microstepping, microstep_conf_t conf)
{
    this->_motor                           = motor;

    memset(this->_motor->motor_name, 0, 12);

    this->_motor->pinout.step_pin          = pins[0];
    this->_motor->pinout.direction_pin     = pins[1];
    this->_motor->pinout.sleep_pin         = pins[2];
    this->_motor->pinout.reset_pin         = pins[3];
    this->_motor->pinout.motor_select1_pin = pins[4];
    this->_motor->pinout.motor_select2_pin = pins[5];
    this->_motor->pinout.motor_select3_pin = pins[6];

    this->_motor->gear_count               = gears;
    this->_motor->rpm                      = rpm;
    this->_motor->microstepping_enabled    = enable_microstepping;
    this->_motor->conf                     = conf;

    this->_motor->default_callback         = nullptr;
    this->_motor->custom_callback          = nullptr;

    this->init();
}


/**
 * @brief Method to init all the basic functions onto the motor struct
 * 
 * @return motor_err_t
 */
motor_err_t MotorDriver::init()
{
    // Enable all the pins needed
    pinMode(this->_motor->pinout.step_pin, OUTPUT);
    pinMode(this->_motor->pinout.direction_pin, OUTPUT);
    pinMode(this->_motor->pinout.sleep_pin, OUTPUT);
    pinMode(this->_motor->pinout.reset_pin, OUTPUT);
    pinMode(this->_motor->pinout.motor_select1_pin, OUTPUT);
    pinMode(this->_motor->pinout.motor_select2_pin, OUTPUT);
    pinMode(this->_motor->pinout.motor_select3_pin, OUTPUT);

    // Configure microstepping
    if (!this->_motor->microstepping_enabled) {
        digitalWrite(this->_motor->pinout.motor_select1_pin, LOW);
        digitalWrite(this->_motor->pinout.motor_select2_pin, LOW);
        digitalWrite(this->_motor->pinout.motor_select3_pin, LOW);
    } else {
        if (this->_motor->conf == MICROSTEP_NONE) {
            digitalWrite(this->_motor->pinout.motor_select1_pin, LOW);
            digitalWrite(this->_motor->pinout.motor_select2_pin, LOW);
            digitalWrite(this->_motor->pinout.motor_select3_pin, LOW);
        }
    }

    // Assign control function as interrupt

    // Calculate the required timing for the 
}