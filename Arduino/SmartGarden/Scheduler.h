#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "Task.h"

#define MAX_TASKS 50

class Scheduler {
  int basePeriod;
  int maxTasks;
  Task* taskList[MAX_TASKS];  

public:
  void init(int basePeriod);  
  virtual bool addTask(Task* task);  
  virtual void schedule();

};

#endif