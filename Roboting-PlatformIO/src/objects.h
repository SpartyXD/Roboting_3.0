#ifndef MOTORS_H
#define MOTORS_H

#include <misc.h>
#include <Adafruit_NeoPixel.h>


struct MotorShield{
    int left_pwm_pin, left_a_pin, left_b_pin;
    int right_pwm_pin, right_a_pin, right_b_pin;
    int MAX_SPEED = 255;

    MotorShield(){}

    void init(int pwm_A, int a_1, int a_2, int pwm_B, int b_1, int b_2, int max_speed=255){
        left_pwm_pin = pwm_A;
        left_a_pin = a_1;
        left_b_pin = a_2;
        
        right_pwm_pin = pwm_B;
        right_a_pin = b_1;
        right_b_pin = b_2;
        
        MAX_SPEED = max_speed;
        
        pinMode(left_pwm_pin, OUTPUT); pinMode(left_a_pin, OUTPUT); pinMode(left_b_pin, OUTPUT);
        pinMode(right_pwm_pin, OUTPUT); pinMode(right_a_pin, OUTPUT); pinMode(right_b_pin, OUTPUT);
        stopMotors();
    }

    void stopMotors(){
        controlMotors(0, 0);
    }

    void setMotorSpeed(int motor, int speed){
        bool reverse = speed<0;
        speed = constrain(abs(speed), 0, MAX_SPEED);

        if(motor == 0){
            //Left
            analogWrite(left_pwm_pin, speed);
            digitalWrite(left_a_pin, !reverse);
            digitalWrite(left_b_pin, reverse);
        }
        else{
            //Right
            analogWrite(right_pwm_pin, speed);
            digitalWrite(right_a_pin, !reverse);
            digitalWrite(right_b_pin, reverse);
        }
    }

    void controlMotors(int left_speed, int right_speed){
        setMotorSpeed(0, left_speed);
        setMotorSpeed(1, right_speed);
    }

};


struct PixelRing{
    int num_pixels = 0;
    int pin = 0;
    bool turned_on = false;
    uint8_t current_color[3] = {0, 0, 0};

    Adafruit_NeoPixel ring;

    void _setPixel(int i, uint8_t r, uint8_t g, uint8_t b){
        ring.setPixelColor(i, ring.Color(r, g, b));
    }

    PixelRing(){}

    void init(int pixels, int pin, int r=0, int g=0, int b=0){
        this->pin = pin;
        num_pixels = pixels;
        current_color[0] = r; current_color[1]= g; current_color[2] = b;

        ring = Adafruit_NeoPixel(num_pixels, pin, NEO_GRB + NEO_KHZ800);
        ring.begin();
        setBrightness(255);
        toggle();
    }

    void setBrightness(int brightness){
        ring.setBrightness(brightness);
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b){
        //Turned off
        if(r == 0 && g == 0 && b == 0){
            turnOff();
            return;
        }

        turned_on = true;
        current_color[0] = r; current_color[1]= g; current_color[2] = b;
        rep(i, num_pixels)
            _setPixel(i, r, g, b);
        
        ring.show();
    }

    void turnOff(){
        turned_on = false;
        ring.clear();
        ring.show();
    }

    void toggle(){
        turned_on = !turned_on;

        if(turned_on)
            setColor(current_color[0], current_color[1], current_color[2]);
        else
            turnOff();
    }

};



#endif