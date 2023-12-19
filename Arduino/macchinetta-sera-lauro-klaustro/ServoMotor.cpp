#include "ServoMotor.h"
#include "Arduino.h"


ServoMotor::ServoMotor(int pin){
  this->pin = pin;
   
} 

void ServoMotor::on(){
  //metodo per collegare il pin
  motor.attach(this->pin);   
}

void ServoMotor::incPosition(){
  //metodo per ruotare di una posizione il servo
  int val=motor.read();
  motor.write((int) val+8.3);          
}

void ServoMotor::setPosition(int val){
  //settaggio della rotazione del servo
  val = 750 + (val*8.3);
  motor.write(val);    
}

int ServoMotor::getPosition(){
  //ottieni l'angolo di rotazione del servo
  int val=motor.read();   
  return ((val - 750) / 8.3 >= 180 ? 180 :(val - 750) / 8.3);
}

void ServoMotor::reset(){ 
  //imposti la posizione del servo a 0
  motor.write(750);  
}



void ServoMotor::off(){
  //scolleghi il servo
  motor.detach();   
}
