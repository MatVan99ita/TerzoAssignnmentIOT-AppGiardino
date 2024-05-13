#ifndef __ILLUMINATIONTASK__
#define __ILLUMINATIONTASK__

#include "Task.h"
#include "Led_switch.h"
#include "Led_fade.h"

class IlluminationTask: public Task {
  Led* led_switch1;
  Led* led_switch2;
  Led* led_f1;
  Led* led_f2;

  enum { CHANGE_ILLUMINATION_STATUS } state;

public:

  LedTask();
  void init(int period);  
  void tick();
};

#endif
