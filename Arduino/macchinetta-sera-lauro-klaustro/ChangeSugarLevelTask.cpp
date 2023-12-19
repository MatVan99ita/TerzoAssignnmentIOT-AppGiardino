#include "ChangeSugarLevelTask.h"
#include "Arduino.h"
#include "Config.h"
/** task che si occupa della modifica del livello di zucchero */

int sugarLevel;

ChangeSugarLevelTask::ChangeSugarLevelTask(int potPin){
    this->potPin = potPin;
}

void ChangeSugarLevelTask::init(int period){
    Task::init(period);
    potentiometer = new Potentiometer(this -> potPin);
    states = UPDATE_LEVEL;
}

void ChangeSugarLevelTask::tick(){
    switch(states){
        case UPDATE_LEVEL:
          if(isSugarSettable){
            sugarLevel = map(potentiometer->getValue(), 0,1023,0,6);
          }
        break;
    }
}
