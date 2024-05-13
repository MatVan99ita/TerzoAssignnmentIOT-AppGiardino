#include "Led.h"
#include "Arduino.h"


#define FADE_VELOCITY 5

void Led:: Led(int pin) {
    this->pin = pin
    pinMode(pin,OUTPUT);
}

void Led::switchOff(){
      digitalWrite(pin, LOW);
}


void Led::switchOn(){
      digitalWrite(pin, HIGH);
}


bool Led::change(){
  if(!led_on){
    this->switchOff();
  }else{
    this->switchOn();
  }
}


void Led::fade(int v){
      val=(val)*FADE_VELOCITY;
      analogWrite(pin, val);
}