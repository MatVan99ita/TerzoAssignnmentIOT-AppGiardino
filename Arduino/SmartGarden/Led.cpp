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
      delay(1);
}


void Led::switchOn(){
      digitalWrite(pin, HIGH);
      delay(1);
}


bool Led::change(){
  if(!led_on){
    Serial.println("Si spengono le luci (Si svegli il lupo)");
    this->switchOff();
  }else{
    Serial.println("Si accendono le luci (E' morto paolo)");
    this->switchOn();
  }
}

void Led::fade(int val){
      val=(val)*64.00;
      analogWrite(this->pin, val);
      delay(1);
}
