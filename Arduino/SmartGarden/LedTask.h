#ifndef __ILLUMINATIONTASK__
#define __ILLUMINATIONTASK__

#include "Task.h"
#include "Led_switch.h"
#include "Led_fade.h"

class IlluminationTask: public Task {
  Led_switch* led_s1;
  Led_switch* led_s2;
  Led_fade* led_f1;
  Led_fade* led_f2;

  enum { CHANGE_ILLUMINATION} state;

public:

  IlluminationTask();  
  void init(int period);  
  void tick();
};

#endif
