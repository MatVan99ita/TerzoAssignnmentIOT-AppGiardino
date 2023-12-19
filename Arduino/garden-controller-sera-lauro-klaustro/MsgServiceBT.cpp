#include "Arduino.h"
#include "MsgServiceBT.h"


MsgServiceBT::MsgServiceBT(int txdPin, int rxdPin){
  channel = new SoftwareSerial(txdPin, rxdPin);
}

void MsgServiceBT::init(){
  content.reserve(256);
  channel->begin(9600);
  availableMsg = NULL;
}

bool MsgServiceBT::sendMsg(Msg msg){
  channel->println(msg.getContent());  
}

bool MsgServiceBT::isMsgAvailable(){
  if(channel->available()!=0){
    Serial.println(channel->available());
  }
  while (channel->available()) {
    Serial.println("aviable msg");
    char ch = (char) channel->read();
    if (ch == '\n'){
      availableMsg = new Msg(content); 
      content = "";
      return true;    
    } else {
      Serial.print(ch);
      content += ch;      
    }
  }
  return false;  
}

Msg* MsgServiceBT::receiveMsg(){
  if (availableMsg != NULL){
    Msg* msg = availableMsg;
    availableMsg = NULL;
    return msg;  
  } else {
    return NULL;
  }
}
