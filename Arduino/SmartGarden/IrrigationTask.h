#ifndef __IRRIGATIONTASK__
#define __IRRIGATIONTASK__

#include "Task.h"
#include "Servo.h"
#include <string>

class IrrigationTask: public Task {
  Servo* servo;
  string servo_state

public:
  IrrigationTask();
  void init(int period);  
  void tick();
};

#endif
