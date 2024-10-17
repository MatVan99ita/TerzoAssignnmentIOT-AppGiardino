#include "Scheduler.h"
#include "Arduino.h"
#include "Config.h"

#include "CommService.h"
#include "CommTask.h"
#include "LedTask.h"
#include "IrrigationTask.h"

SoftwareSerial MyBlue(3, 2); // RX | TX 

//LEDAUTO_3_2
Scheduler sched;
String led_type; //LEDB - LEDF
int led_id;
bool led_on = false;
bool led_max = false;
int servo_speed=0;

void setup() 
{
  sched.init(10);

  Task* ledTask = new LedTask();
  ledTask->init(100);
  ledTask->setActive(false);

  Task* irrigationTask = new IrrigationTask();
  irrigationTask->init(100);
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
