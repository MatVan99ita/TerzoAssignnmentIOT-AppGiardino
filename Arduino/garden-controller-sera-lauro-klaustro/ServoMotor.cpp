#include "ServoMotor.h"
#include "Arduino.h"


ServoMotor::ServoMotor(int pin){
  this->pin = pin;
   
} 

void ServoMotor::on(){
  //metodo per collegare il pin
  motor.attach(this->pin);   
}
/*
void ServoMotor::incPosition(){
  //metodo per ruotare di una posizione il servo
  int val=motor.read();
  motor.write((int) val+8.3);          
}*/

void ServoMotor::setSpeed_s(int val){
  //settaggio della rotazione del servo
  this->speed_s=val;
  //val = 750 + (val*8.3);
  //motor.write(val);    
}

void ServoMotor::startIrrigation(){
  //ottieni l'angolo di rotazione del servo
  //int val=motor.read();   
  //return ((val - 750) / 8.3 >= 180 ? 180 :(val - 750) / 8.3);
  int wait=60-(this->speed_s * 10);
  int angle;
  // Sweep from 0 to 180 degrees:
  for (angle = 0; angle <= 180; angle += 1) {
    motor.write(750+(angle*8.3));
    //Serial.println("avanti");
    delay(wait);
  }
  // And back from 180 to 0 degrees:
  for (angle = 180; angle >= 0; angle -= 1) {
    motor.write(750+(angle*8.3));
    //Serial.println("indietro");
    delay(wait);
  }
  //delay(wait*10);
  
}




void ServoMotor::off(){
  //scolleghi il servo
  motor.detach();
}
