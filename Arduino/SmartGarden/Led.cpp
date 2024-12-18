#include "Led.h"
#include "Arduino.h"
#include "Setting.h"

Led::Led(int pin) {
    this->pin = pin;
    pinMode(pin, OUTPUT);
}


bool Led::change(){
  Serial.println(fade_value);
  if(fade_value > 0) { // is night everybody close theyr eyes... the wolf open his eyes
    digitalWrite(pin, HIGH);
  } else { // the wolf close his eyes, is day... genoveffo died
    digitalWrite(pin, LOW);
  }
}



void Led::fade(int val){
  Serial.println(pin);
  //Take the mapped value and re-approximate to a fading value
  int i = map(val, 0, 4, 0, 255);
  Serial.print("Val: "); Serial.print(val); Serial.print(", i: "); Serial.println(i);
  analogWrite(pin, i);
}
