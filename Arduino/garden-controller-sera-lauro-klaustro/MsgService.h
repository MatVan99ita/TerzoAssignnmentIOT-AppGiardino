#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "Config.h"

class Msg {
  String content;

public:
  Msg(String content){
    this->content = content;
  }
  
  String getContent(){
    return content;
  }
};

class MsgServiceSerial {
    
public: 
  
  Msg* currentMsg;
  bool msgAvailable;

  void init();  

  bool isMsgAvailable();
  Msg* receiveMsg();

  void sendMsg(const String& msg);
};

class MsgServiceBluetooth {
    
public: 

  Msg* currentMsg;
  bool msgAvailable;

  void init();  

  bool isMsgAvailable();
  Msg* receiveMsg();  
 
  void sendMsg(const String& msg);
  SoftwareSerial* channel;
};

extern MsgServiceSerial MsgService;
extern MsgServiceBluetooth MsgServiceBT;

#endif
