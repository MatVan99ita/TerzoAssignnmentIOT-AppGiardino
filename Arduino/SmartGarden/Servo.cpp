#include "Servo.h"
#include "Arduino.h"


void Servo::Servo(int pin) {
    this->pin = pin
}

void Servo::on() {
    motor.attach(this->pin)
}

void Servo::off() {
    motor.detach()
}

void Servo::setVelocity(int val) {
    this->velocity = velocity
}

void Servo::startRotation() {
    int i;
    int delay = 60 - (this->velocity * 10)
    for(i = 0; i < 180; i+= 1) {
        motor.write(750+(i*10));
        delay(delay);
    }
    for(int i = 180; i >= 0; i-= 1) {
        motor.write(750+(i*10));
        delay(delay);
    }
}