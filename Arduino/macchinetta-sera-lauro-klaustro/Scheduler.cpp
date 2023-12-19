#include "Scheduler.h"
#include <TimerOne.h>
#include "Config.h"

bool isSleeping;
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
  nTasks = 0;
}

/** aggiunge una task allo scheduler */
bool Scheduler::addTask(Task* task){
  if (nTasks < MAX_TASKS-1){
    taskList[nTasks] = task;
    nTasks++;
    return true;
  } else {
    return false; 
  }
}

/** effettua lo scheduling delle task */
void Scheduler::schedule(){   
  while (!timerFlag){
    }
  timerFlag = false;
  for (int i = 0; i < nTasks; i++){
    if (taskList[i]->isActive() && taskList[i]->updateAndCheckTime(basePeriod)){
      taskList[i]->tick();
    }
  }
}
