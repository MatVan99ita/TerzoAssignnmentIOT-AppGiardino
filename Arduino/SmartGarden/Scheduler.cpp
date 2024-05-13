#include "Scheduler.h"
#include <TimerOne.h>

volatile bool timerFlag;

void timerHandler(void){
  timerFlag = true;
}

void Scheduler::init(int basePeriod){
  this->basePeriod = basePeriod;
  timerFlag = false;
  long period = 1000l*basePeriod;
  Timer1.initialize(period);
  Timer1.attachInterrupt(timerHandler);
  maxTasks = 0;
}

bool Scheduler::addTask(Task* task){
  if (maxTasks < MAX_TASKS-1){
    taskList[maxTasks] = task;
    maxTasks++;
    return true;
  } else {
    return false; 
  }
}
  
void Scheduler::schedule(){   
  while (!timerFlag){}
  timerFlag = false;

  for (int i = 0; i < maxTasks; i++){
    if (taskList[i]->isActive() && taskList[i]->updateAndCheckTime(basePeriod)){
      taskList[i]->tick();
    }
  }
}
