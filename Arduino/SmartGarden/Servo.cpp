#include "Servo.h"
#include "Arduino.h"


Servo::Servo(int pin) {
    this->pin = pin;
}

void Servo::on() {
    motor.attach(this->pin);
}

void Servo::off() {
    motor.detach();
}

void Servo::setSpeed(int val) {
    this->speed = val;
}

void Servo::startRotation() {
    int pos = 0;
    if(this->speed > 0){//check for the delay equation

      int dilei = map(this->speed, 0, 4, MIN_VAL, MAX_VAL);
      
      for(pos = 0; pos <= 180; pos++) {
        motor.write(pos);
        delay(dilei);
      }

      for(pos = 180; pos >= 0; pos--) {
        motor.write(i);
        delay(dilei);
      }
    }
    
}
