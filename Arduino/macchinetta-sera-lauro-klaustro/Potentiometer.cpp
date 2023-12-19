#include "Potentiometer.h"
#include "Arduino.h"
#include "Config.h"

Potentiometer::Potentiometer(int pin){
    this->pin = pin;
}

/** ritorna il valore attuale del potenziometro */
int Potentiometer::getValue(){
    return analogRead(this->pin);
}
