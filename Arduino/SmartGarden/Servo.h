#ifndef __SERVO__
#define __SERVO__
#include <ServoTimer2.h>

#define MIN_VAL 300  // <- min ms delay
#define MAX_VAL 3000 // <- max ms delay

class Servo {

  public:
    Servo(int pin);
    void on();
    void off();
    void setSpeed(int val);
    void startRotation();
      
  private:
    int pin; 
    int speed;
    ServoTimer2 motor;
};

#endif
