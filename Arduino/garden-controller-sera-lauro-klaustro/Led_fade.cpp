#include "Led_fade.h"
#include "Arduino.h"
#include "Config.h"

Led_fade::Led_fade(int pin){
    this->pin = pin;
    pinMode(pin,OUTPUT);
}

void Led_fade::fade(int val){
      val=(val)*63.75;
      //Serial.println(val);
      analogWrite(pin, val);
}
