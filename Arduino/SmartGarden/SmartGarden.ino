#include "Scheduler.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Config.h"

#include "SoftwareSerial.h"
SoftwareSerial MyBlue(3, 2); // RX | TX 


Scheduler sched;
String led_type;
int led_id;
bool led_on;
int servo_speed=0;

/**
 * TODO: Implementare la ComunicationTask e aggiungere tutte le classi di ocmunicazione del BT
*/

void setup() 
{
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

void loop() 
{ 
  sched.schedule();
}
