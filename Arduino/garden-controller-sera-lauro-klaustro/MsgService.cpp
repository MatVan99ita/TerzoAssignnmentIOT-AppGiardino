#include "Arduino.h"
#include "MsgService.h"

String content;

MsgServiceSerial MsgService;
MsgServiceBluetooth MsgServiceBT;

bool MsgServiceSerial::isMsgAvailable(){
  return msgAvailable;
}

Msg* MsgServiceSerial::receiveMsg(){
  if (msgAvailable){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
}

void MsgServiceSerial::init(){
  Serial.begin(9600);
  content.reserve(128);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;  
}

void MsgServiceSerial::sendMsg(const String& msg){
  Serial.println(msg);  
}

bool MsgServiceBluetooth::isMsgAvailable(){
  /*
  if(!channel->available()){
    Serial.println(channel->available());
  }*/
  //Serial.println("ceck");
  while (channel->available()) {
    Serial.println("aviable msg");
    char ch = (char) channel->read();
    if (ch == '\n'){
      Serial.print(content);
      currentMsg = new Msg(content); 
      content = "";
      msgAvailable=true;    
      return msgAvailable;  
    } else {
      Serial.print(ch);
      content += ch;      
    }
  }
  return false;  
  //return msgAvailable;
}

Msg* MsgServiceBluetooth::receiveMsg(){
  if (msgAvailable){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
}

void MsgServiceBluetooth::init(){
  channel = new SoftwareSerial(BLUE_TXD, BLUE_RXD);
  channel->begin(9600);
  content.reserve(128);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;  
}

void MsgServiceBluetooth::sendMsg(const String& msg){
  channel->println(msg);
}


void serialEvent() {
  /* reading the content */
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == '\n'){
      if (content.length() > 0) {
        int index = content.indexOf('$');        
        if (index != -1){
          /*
          content = content.substring(0,index);
          MsgServiceBT.currentMsg = new Msg(content);
          MsgServiceBT.msgAvailable = true;     
          */ 
        } else {
          MsgService.currentMsg = new Msg(content);
          MsgService.msgAvailable = true;      
        }
      }
    } else {
      content += ch;      
    }
  }
}
