#ifndef __IRRIGATIONTASK__
#define __IRRIGATIONTASK__

#include "Task.h"
#include "Servo.h"

class IrrigationTask: public Task {
  Servo* servo;

public:
  IrrigationTask();
  void init(int period);  
  void tick();
};

#endif
