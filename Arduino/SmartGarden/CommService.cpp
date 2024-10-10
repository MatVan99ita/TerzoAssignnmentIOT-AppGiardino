#include "CommService.h"
#include "Arduino.h"
#include "Config.h"

String content;

SerialService MsgService;
BluetoothService MsgServiceBT;

void serialEvent() {
  Serial.println("PORCODDIO");
  /* reading the content */
  while (Serial.available()) {
    Serial.println(Serial.read());
    char ch = (char) Serial.read();
    if (ch == '\n'){
      if (content.length() > 0) {
        int index = content.indexOf('$');
        if (index != -1){
          
          Serial.println("c'ho il cazzo storto");
          /*
          content = content.substring(0,index);
          MsgServiceBT.currentMsg = new Msg(content);
          MsgServiceBT.msgAvailable = true;
          */
        } else {
          
          Serial.println("CHRI SCARSO SENZA GOLD");
          MsgService.currentMsg = new Msg(content);
          MsgService.msgAvailable = true;
        }
      }
    } else {
      content += ch;
    }
  }
}


void SerialService::init(){
  Serial.begin(9600);
  content.reserve(128);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;
  serialEvent();
}

bool SerialService::isMsgAvailable(){
    return msgAvailable;
}

Msg* SerialService::receiveMsg(){
    if (msgAvailable){
      Msg* msg = currentMsg;
      Serial.println("AVALEIBOL");
      Serial.println(msg->getContent());
      msgAvailable = false;
      currentMsg = NULL;
      content = "";
      return msg;  
  } else {
    Serial.println("not AVALEIBOL");
    return NULL;
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



