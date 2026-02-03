#ifndef MOTORS_H
#define MOTORS_H

#include <misc.h>
#include <Adafruit_NeoPixel.h>


struct VoltageSensor{
    int pin;
    const float VOLTAGE_FACTOR = 5.018;
    float pin_max_voltage = 3.3;
    float resolution = 4095.0;
    float drop = 0.14;

    float current_reading;
    float current_voltage;

    VoltageSensor(){}

    void init(int pin, float pin_max_voltage=-1, float resolution=-1, float drop=-1){
        this->pin = pin;
        pinMode(pin, INPUT);

        if(pin_max_voltage != -1)
            this->pin_max_voltage = pin_max_voltage;
        if(resolution != -1)
            this->resolution = resolution;
        if(drop != -1)
            this->drop = drop;
    }

    float get_voltage(){
        current_reading = analogRead(pin);
        current_voltage = (current_reading/resolution)*pin_max_voltage + drop;
        Serial.println("PinVoltage: " + String(current_voltage) + "\tReading: " + String(current_reading) + "\n");
        current_voltage *= VOLTAGE_FACTOR;
        current_voltage = roundf(current_voltage*100.0) / 100.0;
        return current_voltage;
    }
};


struct MotorShield{
    int left_enable_pin, left_a_pin, left_b_pin;
    int right_enable_pin, right_a_pin, right_b_pin;
    int MAX_SPEED = 255;

    int left_speed = 0;
    int right_speed = 0;

    MotorShield(){}

    void init(int pwm_A, int a_1, int a_2, int pwm_B, int b_1, int b_2, int max_speed=255){
        left_enable_pin = pwm_A;
        left_a_pin = a_1;
        left_b_pin = a_2;
        
        right_enable_pin = pwm_B;
        right_a_pin = b_1;
        right_b_pin = b_2;
        
        MAX_SPEED = max_speed;
        
        pinMode(left_enable_pin, OUTPUT); pinMode(left_a_pin, OUTPUT); pinMode(left_b_pin, OUTPUT);
        pinMode(right_enable_pin, OUTPUT); pinMode(right_a_pin, OUTPUT); pinMode(right_b_pin, OUTPUT);
        stopMotors();
    }

    
    void stopMotors(){
        digitalWrite(left_enable_pin, LOW);
        digitalWrite(right_enable_pin, LOW);
        controlMotors(0, 0);
    }

    void setMotorSpeed(int motor, int speed){
        bool reverse = speed<0;
        speed = constrain(abs(speed), 0, MAX_SPEED);

        if(motor == 0){
            //Left
            digitalWrite(left_enable_pin, HIGH);

            if(!reverse){
                analogWrite(left_a_pin, speed);
                analogWrite(left_b_pin, 0);
            }
            else{
                analogWrite(left_a_pin, 0);
                analogWrite(left_b_pin, speed);             
            }
        }
        else{
            //Right
            digitalWrite(right_enable_pin, HIGH);

            if(!reverse){
                analogWrite(right_a_pin, speed);
                analogWrite(right_b_pin, 0);
            }
            else{
                analogWrite(right_a_pin, 0);
                analogWrite(right_b_pin, speed);             
            }
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