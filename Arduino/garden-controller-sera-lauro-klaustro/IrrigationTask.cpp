#include "IrrigationTask.h"
#include "Arduino.h"
#include "Config.h"

IrrigationTask::IrrigationTask(){
  servo = new ServoMotor(SERVO_MOTOR_PIN);
  
}
  
void IrrigationTask::init(int period){
  Task::init(period);
  servo->on();
  state = SET_SPEED;    
}

void IrrigationTask::tick(){
  switch (state){
    case SET_SPEED:
//      Serial.println("entro speed");
      this->servo->setSpeed_s(value);
      this->state = IRRIGATE;
      break;

    case IRRIGATE:
//      Serial.println("entro irrigate");
      this->servo->startIrrigation();
      this->state = SET_SPEED;
      break;
  }
}
