#ifndef __COMMSERVICE__
#define __COMMSERVICE__

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

class SerialService {
    
public: 
  
  Msg* currentMsg;
  bool msgAvailable;

  void init();  

  bool isMsgAvailable();
  Msg* receiveMsg();

  void sendMsg(const String& msg);
};

class BluetoothService {
    
public: 

  Msg* currentMsg;
  bool msgAvailable;

  void init();  

  bool isMsgAvailable();
  Msg* receiveMsg();  
 
  void sendMsg(const String& msg);
  SoftwareSerial* channel;
};

extern SerialService MsgService;
extern BluetoothService MsgServiceBT;

#endif
