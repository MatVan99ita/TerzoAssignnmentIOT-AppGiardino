#include "IrrigationTask.h"
#include "Arduino.h"
#include "Setting.h"

#define SERVO_PIN 7

IrrigationTask::IrrigationTask(){
    this -> servo = new Servo(SERVO_PIN);
}

void IrrigationTask::init(int period){
    Task::init(period);
}

void IrrigationTask::tick(){
  this -> servo->on();
  this -> servo->setVelocity(servo_speed);
  this -> servo->startRotation();
  this -> servo->off();
}
