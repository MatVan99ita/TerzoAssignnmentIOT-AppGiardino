#include "CommTask.h"

CommTask::CommTask(Task t0, Taskt1){
  IrrigationTask = t0;
  LedTask = t1;
}

void CommTask::init(int period){
  Serial.begin(9600);
  while (!Serial){}
  serialMsg.init();
  btMsg.init();
  Serial.println("ready to go.");
  Task::init(period);
  state=CHEK_NEW_MESSAGE;
  }

/**
    FORMAT:
    [DEVICE]_[PIN]_[VALUE]
    DEVICE:
      LEDB,
      LEDF,
      IRRI
    PIN:
      Only 1, 2 (or 3 for fading both)
    VALUE:
      Light intensity for fading -> <value>
      Intensity of irrigation or activation/deactivation -> ON/OFF/<value>
  */
  void CommTask::tick(){
    switch (state)
    {
    case CHECK_NEW_MESSAGE
      if (serialMsg.isMsgAvailable()) {
        this->msg = serialMsg.receiveMsg();
        this->state = EVALUATE_MESSAGE;
        serialMsg.sendMsg("S Ricevuto");
      }
      if (btMsg.isMsgAvailable()) {
        this->msg = btMsg.receiveMsg();
        this->state = EVALUATE_MESSAGE;
        btMsg.sendMsg("BT Ricevuto");
      }
      break;
    
    case EVALUATE_MESSAGE:
      Serial.print("entro evaluate : ");
      
      msg->getContent().toCharArray(buf, 50);
      
      Serial.println(buf);
      device = String(strtok(buf,"_"));

      switch (device)
      {
        
      case "ERROR"
        btMsg.sendMsg("ERROR");
        break;
        
      case "LEDF"
        led_type=device;
        String fadeLed = String(strtok(NULL, "_"));
        if(fadeLed == "1")
        {
          Serial.println("Ledf 1");
          led_id=1;
            //led_f1->fade(String(strtok(NULL, "_")).toInt());
        }else if (fadeLed =="2"){
          Serial.println("Ledf 2");
          led_id=2;
            //led_f2->fade(String(strtok(NULL, "_")).toInt());
        }else if (fadeLed =="3"){
          Serial.println("Ledf 1&2");
          led_id=3;
        }
        value=String(strtok(NULL, "_")).toInt();
        LedTask->setActive(true);
        break;

      case "LEDB"
        led_type=device;
        btMsg.sendMsg("Blink of an eye");
        Serial.println("Blink of an eye");
        if(String(strtok(NULL, "_"))=="1")
        {
          Serial.println("Led 1");
          led_id=1;
        }else{
          Serial.println("Led 2");
          led_id=2;
        }
        String isOn = String(strtok(NULL, "_"));
        Serial.println(isOn);
        if(isOn=="ON")
        {
          Serial.println("Led $d on", led_id);
          led_on = true;
        } else if (isOn=="OFF"){
          Serial.println("Led $d off", led_id);
          led_on = false;
        } else if (isOn==NULL){
          led_on = !led_on;
        }
        LedTask->setActive(true);
        break;

      case "IRRI"
        Serial.println("servo");
        String val = String(strtok(NULL, "_"));
        if(val=="ON"){
          Serial.println("on");
          IrrigationTask->setActive(true);
        }else if(val=="OFF"){
          IrrigationTask->setActive(false);
        }else{
          value=val.toInt();
          IrrigationTask->init(1000-(val.toInt()*50));
        }
        break;

      }

      device = "";
      delete msg;
      this->state = CHEK_NEW_MESSAGE;
      break;
    }
  }