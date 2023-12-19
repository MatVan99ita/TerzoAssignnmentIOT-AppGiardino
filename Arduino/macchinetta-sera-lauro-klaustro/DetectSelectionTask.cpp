#include "DetectSelectionTask.h"
#include "Arduino.h"
#include "Config.h"

/** task che gestisce la pressione dei bottoni */
extern bool isChanged;
extern int currentProduct;

DetectSelectionTask::DetectSelectionTask(int pinUp, int pinDown, int pinMake ){
    this->pinUp = pinUp;
    this->pinDown = pinDown;
    this->pinMake = pinMake;
    this->stateButtonMake=false;
    this->stateButtonDown=false;
    this->stateButtonUp=false;
}

void DetectSelectionTask::init(int period){
    Task::init(period);
    buttonUp = new Button(this -> pinUp);
    buttonDown = new Button(this -> pinDown);
    buttonMake = new Button(this -> pinMake);
    states = NOT_PRESSED;
}

void DetectSelectionTask::tick(){
    switch(this->states){
        case NOT_PRESSED:
        //Stato del bottone non premuto
            stateButtonUp = buttonUp->isPressed();
            stateButtonDown = buttonDown->isPressed();
            stateButtonMake = buttonMake->isPressed();

            if (stateButtonUp != stateButtonDown || (stateButtonMake && !stateButtonUp && !stateButtonDown) ){
                this->states = _PRESSED;
            }
            break;
        case _PRESSED:
            //Stato del bottone premuto
            isChanged = true;
            do{
              if(stateButtonUp){
                  currentProduct = (currentProduct + 1) % 3;
              }else if(stateButtonDown){
                  currentProduct = (currentProduct - 1) % 3;
                  if(currentProduct<0){
                    currentProduct=2;
                  }
              }else if(stateButtonMake && !stateButtonUp && !stateButtonDown){
                make = true;
              }
            }while(quantity[currentProduct]==0); //codice eseguito fino a quando ci sobno prodotti
            this->states = NOT_PRESSED;
            break;
    }
    
}
