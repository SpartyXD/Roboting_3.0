#ifndef MISC_H
#define MISC_H

#include <Arduino.h>

//=====================================
//PINS

//Left motor
#define PWM_A 32
#define A1_PIN 25
#define A2_PIN 33

//Right motor
#define PWM_B 14
#define B1_PIN 26
#define B2_PIN 27

//LEDS
#define NUM_PIXELS 16
#define FRONT_LED 19
#define BACK_LED 18

//=====================================
//MACROS / PARAMETERS / AUX FUNCTIONS
#define rep(i, n) for(int i=0; i<n; i++)
#define MAX_ARDUINO_TIME 3294967295

unsigned long get_time(){
    return (millis()%MAX_ARDUINO_TIME);
}


//=====================================




#endif