#include "Servo.h"
#include "Arduino.h"


Servo::Servo(int pin) {
    this->pin = pin;
}

void Servo::on() {
    motor.attach(this->pin);
}

void Servo::off() {
    motor.detach();
}

void Servo::setSpeed(int val) {
    this->speed = val;
}

void Servo::startRotation() {
    int pos = 0;
    if(this->speed >= 0){//check for the delay equation

      int dilei = map(this->speed, 0, 4, MIN_VAL, MAX_VAL);
      int angle = 0;
      //Angoli notevoli del servo
      int angle0 =   750 + 0   * 10;
      //int angle90 =  750 + 90  * 10;
      int angle180 = 750 + 180 * 10;

      
      motor.write(angle0);

      for(int i = 0; i <= 180; i = i + this->speed * 10){
        angle = 750 + i *10;
        motor.write(angle);
        Serial.print("ANGOLO SU :"); Serial.println(motor.read());
        delay(dilei);
      }

      motor.write(angle180);
      
      for(int i = 180; i >= 0; i = i - this->speed * 10){
        angle = 750 + i *10;
        motor.write(angle);
        Serial.print("ANGOLO GIU :"); Serial.println(motor.read());
        delay(dilei);
      }
      

      

      
     

    }
    
    
}
