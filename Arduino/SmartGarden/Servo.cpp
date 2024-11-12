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

      //Overkill for calculating bbut is self explanatory
      int angle0 =   750 + 0   * 10;
      int angle90 =  750 + 90  * 10;
      int angle180 = 750 + 180 * 10;

      motor.write(angle0);
      Serial.print("ANGOLO :"); Serial.println(motor.read());
      delay(dilei);

      motor.write(angle90);
      Serial.print("ANGOLO :"); Serial.println(motor.read());
      delay(dilei);

      motor.write(angle180);
      Serial.print("ANGOLO :"); Serial.println(motor.read());
      delay(dilei);

      motor.write(angle90);
      Serial.print("ANGOLO :"); Serial.println(motor.read());
      delay(dilei);

      motor.write(angle0);
      Serial.print("ANGOLO :"); Serial.println(motor.read());
      delay(dilei);

      
     

    }
    
    
}
