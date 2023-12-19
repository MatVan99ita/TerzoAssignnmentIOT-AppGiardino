#include "Temperature.h"
#include "Arduino.h"
#include "Config.h"

Temperature::Temperature(int pin){
    this->pin = pin;
}

double Temperature::getTemperature(){
      //ottieni la temperatura
      int value = analogRead(this->pin);      
      double value_in_C = (((((double)value) / 1024) * 5) -0.5) * 100;
      return value_in_C;
}
