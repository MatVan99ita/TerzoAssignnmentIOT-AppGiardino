#ifndef __COMMTASK__
#define __COMMTASK__

#include "Task.h"
#include "SoftwareSerial.h"
#include "MsgService.h"

class ComunicationTask: public Task {
  String device;
  char buf[50];
  Task* taskIrrigation;
  Task* taskIllumination;
  Msg* msg;
  enum { CHEK_NEW_MESSAGE, EVALUATE_MESSAGE} state;

public:

  ComunicationTask(Task *t0, Task *t1);  
  void init(int period);  
  void tick();
};

#endif
