#include "CommService.h"
#include "Arduino.h"
#include "Setting.h"
#include "String.h"

String content;

SerialService MsgService;
BluetoothService MsgServiceBT;



/*Called after loop() automatically*/
void serialEvent() {
  // reading the content char by char
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == '\n'){
      Serial.print("FINITOH: "); Serial.println(content);
      MsgService.msgAvailable = true;
      MsgService.currentMsg = new Msg(content);
      content = "";
      break;
    } else {
      content += ch;
    }
  }
}//*/




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
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
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
    //Serial.println("AVALEIABOL MESSEGJ");
    String s = channel->readString();
    Serial.print("BANANE AZZURRE: "); Serial.println(s);
    //char ch = (char) channel->read();
    if(s.length() >= 6){
      currentMsg = new Msg(s);
      s = "";
      msgAvailable=true;
      return msgAvailable;
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
    Serial.println(msg);
    channel->println(msg);
}
