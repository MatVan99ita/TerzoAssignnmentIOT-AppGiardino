#include "CommService.h"
#include "Arduino.h"
#include "Config.h"

String content;

SerialService MsgService;
BluetoothService MsgServiceBT;

void SerialService::init(){
  Serial.begin(9600);
  content.reserve(128);
  content = "";
  currentMsg = NULL;
  msgAvailable = true;
}

bool SerialService::isMsgAvailable(){
    return msgAvailable;
}

Msg* SerialService::receiveMsg(){
    if (msgAvailable){
      Msg* msg = currentMsg;
      this->sendMsg("AVALEIBOL");
      //msgAvailable = false;
      currentMsg = NULL;
      content = "";
      return msg;  
  } else {
    this->sendMsg("not AVALEIBOL");
    return NULL;
  }
}


void serialEvento() {
  /* reading the content */
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == '\n'){
      if (content.length() > 0) {
        int index = content.indexOf('$');
        if (index != -1){
          
          Serial.println(content);
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

void SerialService::sendMsg(const String& msg){
    Serial.println(msg);
}

void BluetoothService::init(){
  channel = new SoftwareSerial(BLUE_TXD, BLUE_RXD);
  channel->begin(9600);
  content.reserve(128);
  
  content = "";
  currentMsg = NULL;
  msgAvailable = false;
}

bool BluetoothService::isMsgAvailable(){
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
}

Msg* BluetoothService::receiveMsg(){
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

void BluetoothService::sendMsg(const String& msg){
    channel->println(msg);
}



