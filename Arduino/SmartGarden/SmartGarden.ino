#include "Scheduler.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Config.h"

#include "CommService.h"
#include "CommTask.h"
#include "LedTask.h"
#include "IrrigationTask.h"

SoftwareSerial MyBlue(3, 2); // RX | TX 


Scheduler sched;
String led_type; //LEDB - LEDF
int led_id;
bool led_on;
int servo_speed=0;

void setup() 
{
  sched.init(50);

  Task* ledTask = new LedTask();
  ledTask->init(50);
  sched.addTask(ledTask);
  ledTask->setActive(false);
  
  Task* irrigationTask = new IrrigationTask();
  irrigationTask->init(100);
  sched.addTask(irrigationTask);
  irrigationTask->setActive(false);


  Task* commTask = new CommTask(irrigationTask,ledTask);
  commTask->init(50);
  sched.addTask(commTask);
}

void loop() 
{ 
  sched.schedule();
}
