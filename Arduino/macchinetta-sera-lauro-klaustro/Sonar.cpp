#include "Arduino.h"
#include "Sonar.h"
#include "Config.h" 

NewPing sonar( SONAR_TRIG_PIN , SONAR_ECHO_PIN , 40);

Sonar::Sonar(){
  
}

bool Sonar::isNotDetected(){
    //restituisce true se non incontra nessun ostacolo
    return sonar.ping_cm()==0;

    
    }
