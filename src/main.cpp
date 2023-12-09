#include <Arduino.h>
#include <FastLED.h>
#include <ezButton.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "motor_driver.h"

/******************************************************************/
/* Definitions                                                    */
/******************************************************************/
/***********************************/
/* Local definitions               */
/***********************************/
#define PIN_VOLUME ( A0 )
#define PIN_SW1 ( A1 )
#define PIN_SW2 ( A2 )

#define PIN_NEOPIXEL ( D11 )

#define PIN_PHASE ( D2)
#define PIN_SR ( D3 )
#define PIN_PWML ( D5 )
#define PIN_PWMH ( D6 )

#define PIN_FF1 ( D8 )
#define PIN_FF2 ( D9 )

#define NUM_LEDS 2

/***********************************/
/* Local Variables                 */
/***********************************/
CRGB leds[NUM_LEDS];
ezButton button_1(PIN_SW1);
ezButton button_2(PIN_SW2);

int pwm;
float fpwm;

bool direction = false;
bool brake_mode = false;
bool rotation = true;

uint8_t hue;
CHSV color(0, 255, 255);

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

motor_driver motor1(PIN_PWMH, PIN_PWML, PIN_PHASE, PIN_SR, PIN_FF1, PIN_FF2);

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

/***********************************/
/* Global functions                */
/***********************************/
void setup() {
    Serial.begin(115200);
    pinMode(PIN_NEOPIXEL, OUTPUT);
    pinMode(PIN_VOLUME, INPUT);

    FastLED.addLeds<WS2812B, PIN_NEOPIXEL>(leds, NUM_LEDS);
    button_1.setDebounceTime(50);
    button_2.setDebounceTime(50);

    motor1.begin();

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.display();
    delay(1000);
    display.clearDisplay();
    display.display();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Sensor");
    display.display();
    display.invertDisplay(false);
    display.println("line:0x1F"); //dummy
    display.println("slope:725"); //dummy
    display.println("battery:10.5V"); //dummy
    display.println("test"); //dummy
    display.println("abcdefghijklmnopqrstuvwxyz0123456789+-*/=()[]{}<>!@#$%^&*~`"); //dummy
    
    display.setCursor(0,0);
    display.display();
}

void loop() {
    button_1.loop();
    button_2.loop();

    if (button_1.isPressed()) {
        direction = !direction;
    }

    if (button_2.isPressed()) {
        brake_mode = !brake_mode;
    }

    pwm = (analogRead(PIN_VOLUME)*100)/1023;
    if (direction) {
        pwm = -pwm;
    }

    motor1.set(pwm, brake_mode);

    bool ff1 = digitalRead(PIN_FF1);
    bool ff2 = digitalRead(PIN_FF2);
    leds[0] = color.setHSV((millis()/10)%255, 255, 100);
    leds[1] = ff1|ff2 ? CRGB::Red : CRGB::Black;
    FastLED.show();
}


