#include "Pir.h"
#include "Arduino.h"
#include "Config.h"

#define CALIBRATION_TIME_SEC 10

Pir::Pir(int pin){
    this->pin = pin;
    /** calibrazione PIR */
    pinMode(this->pin, INPUT);
    for(int i = 0; i < CALIBRATION_TIME_SEC; i++){
      delay(1000);
    }
    delay(50);
}

/** ritorna true se viene rilevato un ostacolo */
bool Pir::isDetected(){
      return digitalRead(this -> pin) == HIGH;
}
