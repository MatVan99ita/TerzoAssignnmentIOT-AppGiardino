#include "Led.h"
#include "Arduino.h"
#include "Setting.h"


#define FADE_VELOCITY 5
#define FADE_SLEEP 30
int dilei = 5;


Led::Led(int pin) {
    this->pin = pin;
    led_max_on = false;
    Serial.println(pin);
    pinMode(pin, OUTPUT);
}

void Led::switchOff(){
  Serial.println("OFF");
      digitalWrite(pin, LOW);
      delay(30);
      led_max_on = false;
}


void Led::switchOn(){
  Serial.println("ON");
      digitalWrite(pin, HIGH);
      delay(30);
      led_max_on = true;
}


bool Led::change(){
  if(!led_max_on) {
    this->switchOff();
  } else {
    this->switchOn();
  }
}

void Led::fadein(int val) {
  Serial.println("fadeIN");
  for (int fadeValue = 0 ; fadeValue <= val; fadeValue += 1) {
    analogWrite(this->pin, fadeValue);
    delay(30);
  }
  led_max_on = true;
  }

void Led::fadeout(int val) {
  Serial.println("fadeOUT");
  for (int fadeValue = val ; fadeValue >= 0; fadeValue -= 1) {
    analogWrite(this->pin, fadeValue);
    delay(30);
  }
  led_max_on = false;
  }




void Led::fade(int val){

  //Take the mapped value and approximate to a fading value
  int i = map(val, 0, 7, 0, 255);
  
  /*val=(val)*63.75;
  //Serial.println(val);
  analogWrite(pin, i);//*/

  Serial.print("->"); Serial.println(this->led_on);
  if(this->led_on) {
    Serial.println("RAMARRRO");
    this->fadeout(i);
  } else {
    Serial.println("GALLLINA");
    this->fadein(i);
  }//*/
}
