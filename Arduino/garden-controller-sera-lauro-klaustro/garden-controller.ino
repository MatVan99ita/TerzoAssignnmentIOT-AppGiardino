#include "Scheduler.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Config.h"
#include "MsgService.h"
#include "ComunicationTask.h"
#include "IrrigationTask.h"
#include "IlluminationTask.h"


/**variabili globali*/

Scheduler sched;
String led_type;
int led_id;
bool led_on;
int value=0;


/** setup iniziale */
void setup(){

  sched.init(50);
  
  Task* illuminationTask = new IlluminationTask();
  illuminationTask->init(50);
  sched.addTask(illuminationTask);
  illuminationTask->setActive(false);
  
  Task* irrigationTask = new IrrigationTask();
  irrigationTask->init(100);
  sched.addTask(irrigationTask);
  irrigationTask->setActive(false);

  Task* comunicationTask = new ComunicationTask(irrigationTask,illuminationTask);
  comunicationTask->init(50);
  sched.addTask(comunicationTask);

}




/** loop principale */
void loop(){
  sched.schedule();
}
