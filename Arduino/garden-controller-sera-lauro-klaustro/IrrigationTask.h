#ifndef __IRRIGATIONTASK__
#define __IRRIGATIONTASK__

#include "Task.h"
#include "ServoMotor.h"

class IrrigationTask: public Task {
  ServoMotor* servo;
  //int s=0;
  enum { SET_SPEED, IRRIGATE} state;

public:
  IrrigationTask();  
  void init(int period);  
  void tick();
};

#endif
