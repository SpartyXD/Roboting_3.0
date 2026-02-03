#ifndef MISC_H
#define MISC_H

#include <Arduino.h>

//=====================================
//PINS
#define VOLTAGE_PIN 35
#define LOW_VOLTAGE_THRESHOLD 7.60

//Left motor
#define ENABLE_L_PIN 32
#define A1_PIN 14
#define A2_PIN 27

//Right motor
#define ENABLE_R_PIN 26
#define B1_PIN 25 
#define B2_PIN 33 

//LEDS
#define NUM_PIXELS 16
#define FRONT_LED 18
#define BACK_LED 19

//=====================================
//MACROS / PARAMETERS / AUX FUNCTIONS
#define rep(i, n) for(int i=0; i<n; i++)
#define MAX_ARDUINO_TIME 3294967295

unsigned long get_time(){
    return (millis()%MAX_ARDUINO_TIME);
}


//=====================================




#endif