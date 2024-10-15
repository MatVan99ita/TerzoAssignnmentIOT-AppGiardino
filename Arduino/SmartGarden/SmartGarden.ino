#include "Scheduler.h"
#include "Arduino.h"
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
  Task* irrigationTask = new IrrigationTask();
  Task* commTask = new CommTask(irrigationTask,ledTask);
  
  ledTask->init(50);
  irrigationTask->init(100);
  commTask->init(100);
  
  ledTask->setActive(false);
  irrigationTask->setActive(false);
  irrigationTask->setActive(true);
  
  sched.addTask(ledTask);
  sched.addTask(irrigationTask);
  sched.addTask(commTask);
}

void loop() {
  sched.schedule();
}
