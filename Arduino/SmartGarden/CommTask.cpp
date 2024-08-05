#include "ComunicationTask.h"

CommTask::CommTask(Task t0, Taskt1){
  IrrigationTask = t0;
  LedTask = t1;
}

void CommTask::init(int period){
  Serial.begin(9600);
  while (!Serial){}
  MsgService.init();
  MsgServiceBT.init();
  Serial.println("ready to go.");
  Task::init(period);
  state=CHEK_NEW_MESSAGE;
  }