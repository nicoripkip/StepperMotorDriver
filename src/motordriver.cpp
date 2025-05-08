#include "motordriver.hpp"
#include "Arduino.h"


void IRAM_ATTR Motor_Timer_ISR()
{
    // digitalWrite(this->_motor->pinout.step_pin, HIGH);
    // delayMicroseconds(1);
    // digitalWrite(this->_motor->pinout.step_pin, LOW);
}


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

    motor_err_t err = this->init();
    if (err != MOTOR_ERR_OK) {

    }
}


/**
 * @brief Destructor
 * 
 */
MotorDriver::~MotorDriver()
{
    // free(this->_motor->default_callback);
    // free(this->_motor->custom_callback);
    free(this->_motor);
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

    digitalWrite(this->_motor->pinout.direction_pin, LOW);

    // Configure other pins
    digitalWrite(this->_motor->pinout.reset_pin, HIGH);
    digitalWrite(this->_motor->pinout.sleep_pin, HIGH);

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
    this->_motor->use_custom_callback      = false;
    this->_motor->default_callback         = Motor_Timer_ISR;
    this->_motor->custom_callback          = nullptr;

    // Setup timers



    // Calculate the required timing for the 

    return MOTOR_ERR_OK;
}


/**
 * @brief 
 * 
 * 
 */
motor_err_t MotorDriver::assign_motor_name(char* name)
{
    return MOTOR_ERR_OK;
}


/**
 * 
 * 
 */
motor_err_t MotorDriver::assign_custom_interrupt(interrupt_callback callback)
{
    this->_motor->custom_callback = callback;

    this->_motor_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(this->_motor_timer, this->_motor->custom_callback, true);
    timerAlarmWrite(this->_motor_timer, 1000, true);
    timerAlarmEnable(this->_motor_timer);

    return MOTOR_ERR_OK;
}