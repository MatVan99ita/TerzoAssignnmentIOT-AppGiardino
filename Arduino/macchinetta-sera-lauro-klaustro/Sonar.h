#ifndef __SONAR__
#define __SONAR__
#include "Task.h"
#include <NewPing.h>

class Sonar {
  public:
    Sonar();   
    bool isNotDetected();
    
  private:
    int pinEcho;
    int pinTrig;
    
  };

#endif
