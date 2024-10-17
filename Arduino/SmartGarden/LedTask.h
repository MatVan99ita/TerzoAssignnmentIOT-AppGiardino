#ifndef __ILLUMINATIONTASK__
#define __ILLUMINATIONTASK__

#include "Task.h"
#include "Led.h"

class LedTask: public Task {
  Led* led_s1;
  Led* led_s2;
  Led* led_f1;
  Led* led_f2;

  int led_id;
public:

  LedTask();
  void init(int period);
  void tick();
};

#endif
