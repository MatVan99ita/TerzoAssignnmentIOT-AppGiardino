#include "Led.h"
#include "Arduino.h"


#define FADE_VELOCITY 5
#define FADE_SLEEP 30

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


void Led::fadeOut(int v){
    analogWrite(pin, v);
    if(v <= 0)
    v = v - FADE_VELOCITY
    if(v <= 0){
        break;
    }
    sleep(FADE_SLEEP)
    Led::fadeOut(v)
}


voi Led::fadeIn(int v){
    analogWrite(pin, fadeValue);
    v = v + FADE_VELOCITY
    if(v >= 255){
        break;
    }
    sleep(FADE_SLEEP)
    Led::fadeIn(v)
}
