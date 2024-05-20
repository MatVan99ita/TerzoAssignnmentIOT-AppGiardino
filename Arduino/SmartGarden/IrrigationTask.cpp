#include "IrrigationTask.h"
#include "Arduino.h"

#define SERVO_PIN 7

IrrigationTask::IrrigationTask(){
    this -> servo = new Servo(SERVO_PIN)
}

IrrigationTask::init(int period){
    Task::init(period)
}

IrrigationTask::tick(){
    switch(servo_state){
        "Speed":
            this -> servo->setVelocity(servo_speed)
            this -> servo_state = "Irrigation"
            break;
        "Irrigation":
            this -> servo->startRotation()
            this -> servo_state = "Speed"
            break;
    }
}