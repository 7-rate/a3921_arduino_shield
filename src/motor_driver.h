#pragma once
#include <Arduino.h>
#include <pwm.h>
#include "type_defines.h"

/******************************************************************/
/* Definitions                                                    */
/******************************************************************/
/***********************************/
/* Global definitions              */
/***********************************/
#define FORWARD (true)
#define REVERSE (false)

#define BRAKE_MODE (true)
#define COAST_MODE (false)

/***********************************/
/* Class                           */
/***********************************/
class motor_driver {
public:
    motor_driver(pin_size_t pin_h, pin_size_t pin_l, pin_size_t pin_phase, pin_size_t pin_sr, pin_size_t pin_ff1, pin_size_t pin_ff2);
    ~motor_driver();

    void begin();
    void stop();
    void set_frequency(u4 freq_hz);
    void set(s4 pwm, bool brake_mode);

private:
    pin_size_t pin_h;       // A3921 PWMH
    pin_size_t pin_l;       // A3921 PWML
    pin_size_t pin_phase;   // A3921 PHASE
    pin_size_t pin_sr;      // A3921 SR
    pin_size_t pin_ff1;     // A3921 FF1
    pin_size_t pin_ff2;     // A3921 FF2
    bool brake_mode;        // BRAKE_MODE or COAST_MODE
    s4 pwm;                 // -100 to 100
    PwmOut* pwmh;
    PwmOut* pwml;
    bool direction;         // FORWARD or REVERSE
    u4 freq_hz;             // hz

    u4 freq_to_width(u4 freq_hz);
    void set_pwm(PwmOut* out, s4 pwm);

};

/***********************************/
/* Global functions                */
/***********************************/

/***********************************/
/* Global Variables                */
/***********************************/