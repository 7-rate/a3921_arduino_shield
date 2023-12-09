#include "motor_driver.h"
/******************************************************************/
/* Definitions                                                    */
/******************************************************************/
/***********************************/
/* Local definitions               */
/***********************************/
#define DEFAULT_FREQUENCY_HZ ( 5000 )

/***********************************/
/* Local Variables                 */
/***********************************/

/***********************************/
/* Global Variables                */
/***********************************/

/******************************************************************/
/* Implementation                                                 */
/******************************************************************/
/***********************************/
/* Local functions                 */
/***********************************/

/***********************************/
/* Class implementions             */
/***********************************/
/* pin_hとpin_lは同じタイマチャンネルのピンを指定すること！ */
/* さもなくば爆発します                                  */
motor_driver::motor_driver(pin_size_t pin_h, pin_size_t pin_l, pin_size_t pin_phase, pin_size_t pin_sr, pin_size_t pin_ff1, pin_size_t pin_ff2) :
    pin_h(pin_h),
    pin_l(pin_l),
    pin_phase(pin_phase),
    pin_sr(pin_sr),
    pin_ff1(pin_ff1),
    pin_ff2(pin_ff2),
    brake_mode(BRAKE_MODE),
    pwm(0),
    direction(FORWARD)
{
}

motor_driver::~motor_driver() {
    stop();
    delete pwmh;
    delete pwml;
}

void motor_driver::begin() {
    pinMode(pin_h, OUTPUT);
    pinMode(pin_l, OUTPUT);
    pinMode(pin_phase, OUTPUT);
    pinMode(pin_sr, OUTPUT);
    pinMode(pin_ff1, INPUT_PULLDOWN);
    pinMode(pin_ff2, INPUT_PULLDOWN);
    freq_hz = DEFAULT_FREQUENCY_HZ;
    pwmh = new PwmOut(pin_h);
    pwml = new PwmOut(pin_l);
    pwmh->begin(freq_to_width(freq_hz), 0);
    pwml->begin(freq_to_width(freq_hz), 0);
    set_frequency(1000);
    stop();
}

void motor_driver::stop() {
    digitalWrite(pin_h, LOW);
    digitalWrite(pin_l, LOW);
    digitalWrite(pin_phase, LOW);
    digitalWrite(pin_sr, HIGH);
    pwmh->pulse_perc(0);
    pwml->pulse_perc(0);
}

void motor_driver::set(s4 pwm, bool brake_mode) {
    this->pwm = pwm;
    this->brake_mode = brake_mode;

    direction = (pwm > 0) ? FORWARD : REVERSE;

    if (direction == FORWARD) {
        digitalWrite(pin_phase, HIGH);
    } else {
        digitalWrite(pin_phase, LOW);
    }

    if (brake_mode == BRAKE_MODE) {
        set_pwm(pwml, 100);
        set_pwm(pwmh, pwm);
    } else {
        set_pwm(pwml, pwm);
        set_pwm(pwmh, pwm);
    }
}

void motor_driver::set_frequency(u4 freq_hz) {
    this->freq_hz = freq_hz;
    pwmh->period_us(freq_to_width(freq_hz));
    pwml->period_us(freq_to_width(freq_hz));
}

u4 motor_driver::freq_to_width(u4 freq_hz) {
    return (1000000 / freq_hz); // us
}

void motor_driver::set_pwm(PwmOut* out, s4 pwm) {
    s4 abs_pwm = abs(pwm);
    out->pulse_perc(abs_pwm);
}

/***********************************/
/* Global functions                */
/***********************************/