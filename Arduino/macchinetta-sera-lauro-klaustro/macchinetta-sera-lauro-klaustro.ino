#include "Scheduler.h"
#include "Config.h"
#include "ChangeSugarLevelTask.h"
#include "DetectSelectionTask.h"
#include "MakingProductTask.h"
#include "MsgService.h"
#include "Arduino.h"
#include <avr/sleep.h>  
#include "MachineCheckTask.h"

/**variabili globali*/
bool needAssistance; //controlla se c'è bisogno dell'assistenza
bool make; //controlla se è stato premuto il bottone make
bool isChanged; //controlla se è stato premuto o il bottone up o down
int currentProduct; //mantiene il prodotto corrente
bool servoBusy; // gestisce l'utilizzo del servo
int quantity[3] = {1, 1 ,1 }; //mantiene la quantità di ogni prodotto
bool isSugarSettable = false; //controlla se puoi settare lo zucchero

Scheduler sched;

/** sveglia della sleep */
void wakeUp(){
  
}

/** setup iniziale */
void setup(){
  /** inizializzazione variabili globali */
  MsgService.init();
  servoBusy = false;
  isChanged = false;
  make = false;
  isSleeping = false;
  needAssistance = false;
  servoBusy = false;
  needRefill = false;
  sugarLevel = 0;
  canSleep = true;
  
  /** inizializzazione task e periodi */
  Serial.begin(115200);
  sched.init(100);
  Task* detectSelectionTask = new DetectSelectionTask(PIN_UP, PIN_DOWN, PIN_MAKE);
  Task* changeSugarLevelTask = new ChangeSugarLevelTask(POT_PIN);
  Task* makingProductTask = new MakingProductTask();
  Task* machineCheckTask = new MachineCheckTask(PIN_TEMP);

  machineCheckTask->init(100);
  detectSelectionTask->init(100);
  makingProductTask->init(100);
  changeSugarLevelTask->init(1000);
  
  /**aggiunta delle task allo sheduler*/
  sched.addTask(changeSugarLevelTask);
  sched.addTask(detectSelectionTask);
  sched.addTask(makingProductTask);
  sched.addTask(machineCheckTask);
   
}


/** gestione sleep */
void setSleep(){
  
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), wakeUp ,HIGH);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  
  sleep_enable();
  sleep_mode();  
  /** The program will continue from here. **/  
  isSleeping = false;
  /* First thing to do is disable sleep. */  
  detachInterrupt(digitalPinToInterrupt(PIR_PIN));
  sleep_disable(); 

}

/** loop principale */
void loop(){
  if(isSleeping){
    setSleep();
  }
  if(MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg(); 
    if(msg->getContent() == "recovered")  {
       MsgService.sendMsg("arigagrazie");
       needAssistance = false;
    }else if(msg->getContent() == "refilled")  {
       MsgService.sendMsg("arigatou");
       needAssistance = false;
    }
    delete msg;
  }
  sched.schedule();
  
}
