#include "Scheduler.h"
#include "Arduino.h"
#include "Setting.h"

#include "CommService.h"
#include "CommTask.h"
#include "LedTask.h"
#include "IrrigationTask.h"

SoftwareSerial MyBlue(3, 2); // RX | TX 

//LEDAUTO_3_2
Scheduler sched;
String led_type; //LEDAUTO - IRRI - LEDB - LEDF
int led_id;

int servo_speed;
int fade_value;

void setup() 
{
  sched.init(10);

  Task* ledTask = new LedTask();
  ledTask->init(50);
  ledTask->setActive(false);

  Task* irrigationTask = new IrrigationTask();
  irrigationTask->init(100);//Può rimanere 100 percjhè è ogni 100ms che viene controllato cioè ogni 2 tick di commtask poichè è meno frequente come task da eseguire
  irrigationTask->setActive(false);
  
  Task* commTask = new CommTask(irrigationTask, ledTask);
  commTask->init(50);
  commTask->setActive(true);
  
  
  sched.addTask(commTask);
  sched.addTask(ledTask);
  sched.addTask(irrigationTask);
}

void loop() {
  sched.schedule();
  delay(15);
}
