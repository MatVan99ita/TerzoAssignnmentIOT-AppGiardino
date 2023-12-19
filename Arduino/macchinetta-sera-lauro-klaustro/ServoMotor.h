#ifndef __SERVO__
#define __SERVO__
#include <ServoTimer2.h>

class ServoMotor {

  public:
    ServoMotor(int pin);
    void off();
    void on();
    void incPosition();
    void setPosition(int val);
    int getPosition();
    void reset();
      
  private:
    int pin; 
    ServoTimer2 motor;
};

#endif
