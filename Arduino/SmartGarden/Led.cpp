#include "Led.h"
#include "Arduino.h"
#include "Config.h"


#define FADE_VELOCITY 5
#define FADE_SLEEP 30

Led::Led(int pin) {
    this->pin = pin;
    pinMode(pin,OUTPUT);
}

void Led::switchOff(){
      digitalWrite(pin, LOW);
}


void Led::switchOn(){
      digitalWrite(pin, HIGH);
}


bool Led::change(){
  if(led_on){
    this->switchOff();
    led_on = false;
  }else{
    this->switchOn();
    led_on = true;
  }
}

void Led::fade(int val){
      val=(val)*64.00;
      analogWrite(this->pin, val);
}
