#ifndef __COMMTASK__
#define __COMMTASK__

#include "Task.h"
#include "SoftwareSerial.h"
#include "MsgService.h"

class CommTask: public Task {
  String device;
  char buf[50];
  Task* IrrigationTask;
  Task* LedTask;
  Msg* msg;
  enum { CHEK_NEW_MESSAGE, EVALUATE_MESSAGE} state;

public:

  CommTask(Task t0, Taskt1);
  void init(int period);
  void tick();
};

#endif