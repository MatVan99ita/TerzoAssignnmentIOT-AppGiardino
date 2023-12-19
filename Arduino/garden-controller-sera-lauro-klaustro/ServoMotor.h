#ifndef __SERVO__
#define __SERVO__
#include <ServoTimer2.h>

class ServoMotor {

  public:
    ServoMotor(int pin);
    void off();
    void on();
    void setSpeed_s(int val);
    void startIrrigation();
      
  private:
    int pin; 
    int speed_s;
    ServoTimer2 motor;
};

#endif
