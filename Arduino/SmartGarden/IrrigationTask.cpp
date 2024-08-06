#include "IrrigationTask.h"
#include "Arduino.h"
#include "Config.h"

#define SERVO_PIN 7

IrrigationTask::IrrigationTask(){
    this -> servo = new Servo(SERVO_PIN);
    this -> servo_state = SPEED;
}

void IrrigationTask::init(int period){
    Task::init(period);
}

void IrrigationTask::tick(){
    switch(servo_state){
        case SPEED:
            this -> servo->setVelocity(servo_speed);
            this -> servo_state = IRRIGATION;
            break;
        case IRRIGATION:
            this -> servo->startRotation();
            this -> servo_state = SPEED;
            break;
    }
}
