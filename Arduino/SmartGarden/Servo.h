#ifndef __SERVO__
#define __SERVO__
#include <ServoTimer2.h>

class Servo {

  public:
    Servo(int pin);
    void on();
    void off();
    void setVelocity(int val);
    void startRotation();
      
  private:
    int pin; 
    int velocity;
    ServoTimer2 motor;
};

#endif
