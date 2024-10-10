#ifndef __COMMSERVICE__
#define __COMMSERVICE__

#include "Arduino.h"
#include "SoftwareSerial.h"

class Msg {
  String content;

public:
  Msg(String content){
    this->content = content;
    Serial.println(content);
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

  SoftwareSerial* channel;
  Msg* currentMsg;
  bool msgAvailable;
  
  void init();
  bool isMsgAvailable();
  Msg* receiveMsg();
  void sendMsg(const String& msg);
};

extern SerialService MsgService;
extern BluetoothService MsgServiceBT;

#endif
