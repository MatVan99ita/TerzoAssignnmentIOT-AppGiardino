#include "CommService.h"
#include "Arduino.h"
#include "Config.h"
#include "String.h"

String content;

SerialService MsgService;
BluetoothService MsgServiceBT;

/**Read and delete all remaining data in the buffer */
void flushSerialBuffer() {
  while (Serial.available()) {
    Serial.read();
  }
}



/*Called after loop() automatically*/
void serialEvent() {
  content = "";  // Buffer reset
  /* reading the content char by char*/
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == '\n'){
      MsgService.currentMsg = new Msg(content);
      MsgService.msgAvailable = true;
      content = "";  // Buffer reset after receiving message
      break; //End the receiving
    } else {
      content += ch;
    }

    /** 1 Second timeout for infinite waiting preventing */
    if (millis() - start_time > 1000) {
      Serial.println("Timeout: reset del buffer seriale");
      content = "";  // Buffer reset
      break;
    }
  }


}




void SerialService::init(){
  Serial.begin(9600);
  Serial.setTimeout(1);
  content.reserve(128);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;
}

bool SerialService::isMsgAvailable(){
    return msgAvailable;
}

Msg* SerialService::receiveMsg(){
    if (msgAvailable){
      Msg* msg = currentMsg;
      Serial.print("RICEVIUTO: ");
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
    Serial.println("avalaible msg");
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



