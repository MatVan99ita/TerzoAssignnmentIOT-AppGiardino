#include "MachineCheckTask.h"
#include "ServoMotor.h"
#include "Arduino.h"
#include "Config.h"
#include "MsgService.h"

int temperature_value;
bool canSleep;

MachineCheckTask::MachineCheckTask(int temperaturePin){
    this->temperaturePin = temperaturePin;
}

void MachineCheckTask::init(int period){
  //inizializzazione variabili
    Task::init(period);
    this->temperature = new Temperature(this -> temperaturePin);
    states = WAITING_FOR_CHECK;
    MsgService.init();
    this->angle=180;
    this->servo = new ServoMotor(SERVO_MOTOR_PIN);
    this->tickCounter = 0;
    this->period = period;
    this->counter=0;
}

void MachineCheckTask::tick(){
    switch(states){
        case WAITING_FOR_CHECK:
        //aspetta fino a quando non è ora di effettuare il self test
          this->tickCounter++;
          if(needAssistance){
            this->states = GO_ASSISTANCE;
            break;
          }
          if(this->period * this->tickCounter >= T_CHECK && !servoBusy){ //tempo che è passato in millisecondi
                this->tickCounter = 0;
                servoBusy = true;
                canSleep = false;
                this->states = SERVO_CHECK;
          }
          break;
        case SERVO_CHECK:
        //rotazione del servo durante il self test
            this->servo->on();
            if (this->counter == 6){
              this->states = TEMPERATURE_CHECK;
              this->servo->off();
              
              this->counter=0;
            }
            this->servo->setPosition(this->angle);
            if (this->servo->getPosition() == this->angle){
              this->counter++;
              if (counter == 3){
                this->angle = 0;
              }
            }
          break;
        case TEMPERATURE_CHECK:
        //controllo della temperatura
          temperature_value = temperature->getTemperature();
          if(temperature_value >= TEMP_MAX || temperature_value <= TEMP_MIN || needAssistance){
            //ASSISTENZA
            MsgService.sendMsg("recover");
            needAssistance = true;
            needRefill = false;
            this->states = GO_ASSISTANCE;
          }else{
            canSleep = true;
            this->states = WAITING_FOR_CHECK;
          }
          MsgService.sendMsg("test");
          servoBusy = false;
        break;
        case GO_ASSISTANCE:
            //assume questo stato quando va in assistenza
            if(!needAssistance){
              canSleep = true;
              this->states = WAITING_FOR_CHECK;
            }
          
        break;
    }
    
}
