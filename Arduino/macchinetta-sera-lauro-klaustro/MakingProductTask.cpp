#include "MakingProductTask.h"
#include "Arduino.h"
#include "Config.h"
#include "MsgService.h"
/** task principale, gestisce la selezione e produzione della bevanda */

/** possibili motivi per necessitare di assistenza */
bool needRefill;

MakingProductTask::MakingProductTask(){
}

void MakingProductTask::init(int period){
    Task::init(period);
    this->period = period;
    this->displaylcd = new Display();
    this->states = WELCOME;
    this->tickCounter = 0;
    this->sonar = new Sonar();
    this->servoCounter = 0;
    this->servo = new ServoMotor(SERVO_MOTOR_PIN);
    this->pir = new Pir(PIR_PIN);
    this->temperature = new Temperature(PIN_TEMP);
}

void MakingProductTask::tick(){
    /** gestione degli stati presenti nel task */
    
    switch(this->states){
        case WELCOME:
        /** stato di benvenuto */
            this->tickCounter++;
            displaylcd->showString("Welcome to the Smart Coffee Machine!");
            if(this->period * this->tickCounter >= WELCOME_TIME){ //tempo che è passato in millisecondi
                this->tickCounter = 0;
                MsgService.sendMsg("idle");
                isSugarSettable=true;
                this->states = READY;
            } 
            break;
        case READY:
        /** stato in cui la macchinetta attende che l'utente prema un pulsante e controlla se la macchina necessita di assistenza, 
        se ciò non avviene va in sleep mode */
            //Serial.println("READY");
            
            if(needAssistance){
              isSugarSettable=false;
              this->states = ASSISTANCE;
              break;
            }
            this->tickCounter++;
            Serial.println(this->period * this->tickCounter);
            
            displaylcd->showString("Ready!");
            if(this->period * this->tickCounter >= T_IDLE && !isChanged && !make && !servoBusy && canSleep){ 
                this->tickCounter = 0;
                isSugarSettable=false;
                displaylcd->showString("");
                this->states = GO_SLEEP;
                isSleeping = true;
            } else if(isChanged){
                this->tickCounter = 0;
                isSugarSettable=false;
                this->states = SELECT;
            } else if(make && !servoBusy){
                servoBusy = true;
                this->servo->on();
                this->servo->reset();
                MsgService.sendMsg("working");
                isSugarSettable=false;
                this->states = MAKING_PRODUCT;
                make = false;
            }
            break;
        case SELECT:
                /**  stato per la selezione del prodotto */
                isChanged = false;
                displaylcd->showString(beverage[currentProduct]);
                this->states = WAIT;
            break;
        case WAIT:
                /** stato in cui si attende che l'utente selezioni un altro prodotto,
                se ciò non avviene torna allo stato ready*/
                this->tickCounter++;
                if(this->period * this->tickCounter >= WAIT_TIME && !isChanged){ 
                  this->tickCounter = 0;
                  isSugarSettable=true;
                  this->states = READY;
                } else if(isChanged){ //è stato ripremuto il pulsante
                  this->tickCounter = 0;
                  this->states = SELECT;
                } 
            break;
        case GO_SLEEP:         
            /** stato relativo allo sleep mode */
            if(!isSleeping && !needAssistance){
              this->states = WELCOME;
            }else if(!isSleeping && needAssistance){
              this->states = ASSISTANCE;
            }
            break;
        case MAKING_PRODUCT:
            /** stato in cui viene prodotta la bevanda selezionata */
            this->servoCounter++;
            this->servo->setPosition(this->servoCounter);
            this->message = String("Making a " + beverage[currentProduct] + "...");
            displaylcd->showString(this->message);
            if(this->servoCounter >= 180){
                quantity[currentProduct]--;
                this->states = FINISHED_PRODUCT;
            } 
            break;
        case FINISHED_PRODUCT:
            /** fine produzione bevanda */
            this->tickCounter++;
            this->message = String("The " + beverage[currentProduct] + " is ready." );
            displaylcd->showString(this->message);
            if(this->period * this->tickCounter >= T_TIMEOUT || this->sonar->isNotDetected()){
                this->tickCounter = 0;
                this->states = RETIRED_PRODUCT;
                MsgService.sendMsg("decrement");
            }
            break;
        case RETIRED_PRODUCT:
            /** l'utente ritira la bevanda o il timer scade */
            while(quantity[currentProduct] == 0 && quantity[0]+quantity[1]+quantity[2] != 0){ /*quando un prodotto è finito, imposta come prodotto di default il suo successivo*/
              currentProduct = (currentProduct + 1) % 3;
            }
            this->servo->reset();
            this->servoCounter = 0;
            delay(150);
            this->servo->off();
            servoBusy = false;
            
            if(quantity[0] + quantity[1] + quantity[2] == 0){
              
              this->states = ASSISTANCE;
              MsgService.sendMsg("refill");
              needAssistance = true;
              needRefill = true;
            }else{
              MsgService.sendMsg("idle");
              isSugarSettable=true;
              this->states = READY;
            }
        break;
        case ASSISTANCE:
            /** macchinetta in assistenza */
            displaylcd->showString("Assistance required");
            if(!needAssistance){
              if(needRefill){
                quantity[0] = 1;
                quantity[1] = 1;
                quantity[2] = 1;
              }
              isSugarSettable=true;
              this->states = READY;
            }
          
        break;
    }
}
