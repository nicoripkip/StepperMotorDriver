#ifndef INC_MOTOR_DRIVER_HPP
#define INC_MOTOR_DRIVER_HPP


#include "Arduino.h"


typedef enum {
    MOTOR_ERR_OK = 0
} motor_err_t;


typedef enum {
    MICROSTEP_NONE  = 0,
    MICROSTEP_4X    = 1,
    MICROSTEP_16X   = 2
} microstep_conf_t;


typedef motor_err_t (*interrupt_callback)();


typedef struct {
    char                    motor_name[12];

    struct pinout {
        uint8_t             step_pin;               // Yellow 2
        uint8_t             direction_pin;          // Orange 15
        uint8_t             sleep_pin;              // Green 0
        uint8_t             reset_pin;              // Blue 4
        uint8_t             motor_select1_pin;      // Gray 
        uint8_t             motor_select2_pin;      // White 17
        uint8_t             motor_select3_pin;      // Purple 16
    } pinout;

    uint8_t                 gear_count;
    uint8_t                 rpm;
    bool                    microstepping_enabled;
    microstep_conf_t        conf;

    interrupt_callback      default_callback;
    interrupt_callback      custom_callback;
} motor_data_t;


class MotorDriver 
{
public:
    MotorDriver(motor_data_t* motor, int* pins, uint8_t gears, uint8_t rpm, bool enable_microstepping = false, microstep_conf_t conf = MICROSTEP_NONE);
    ~MotorDriver();

    motor_err_t init();
    motor_err_t step();
    motor_err_t assign_motor_name(char* name);

private:
    motor_data_t *_motor;
};


#endif