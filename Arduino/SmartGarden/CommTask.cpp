#include "CommTask.h"
#include "Config.h"


CommTask::CommTask(Task *t0, Task *t1){
  IrrigationTask = t0;
  LedTask = t1;
}

void CommTask::init(int period){
  Serial.begin(9600);
  while (!Serial.available());
  MsgService.init();
  MsgServiceBT.init();
  Serial.println("ready to go.");
  Task::init(period);
  state = CHECK_NEW_MESSAGE;
}

/**
    FORMAT:
    [DEVICE]_[PIN]_[servo_speed]
    DEVICE: cambiare LEDB/F con LED-AUTO LED-MANUAL 
      LEDAUTO,
      LEDMB,
      LEDMF,
      IRRIA,
      IRRIM
    PIN:
      Only 1, 2 or 3 (=for fading both)
    servo_speed:
      Light intensity for fading -> <servo_speed>
      Intensity of irrigation or activation/deactivation -> ON/OFF/<servo_speed>
  */
  void CommTask::tick(){
    switch (state)
    {
      case CHECK_NEW_MESSAGE:
        if (MsgService.isMsgAvailable()) {
          this->msg = MsgService.receiveMsg();
          this->state = EVALUATE_MESSAGE;
          MsgService.sendMsg("S Ricevuto");
        }
        if (MsgServiceBT.isMsgAvailable()) {
          this->msg = MsgServiceBT.receiveMsg();
          this->state = EVALUATE_MESSAGE;
          MsgServiceBT.sendMsg("BT Ricevuto");
        }
        break;

      case EVALUATE_MESSAGE:

        Serial.print("entro evaluate : ");

        msg->getContent().toCharArray(buf, 50);

        Serial.println(buf);
        device = String(strtok(buf,"_"));

        if(device == "ERROR"){
          MsgServiceBT.sendMsg("ERROR");
        } else if(device == "LEDAUTO"){
          led_type=device;
          String fadeLed = String(strtok(NULL, "_"));
          if(fadeLed == '1')
          {
            Serial.println("Ledf 1");
            led_id=1;
              //led_f1->fade(String(strtok(NULL, "_")).toInt());
          }else if (fadeLed =='2'){
            Serial.println("Ledf 2");
            led_id=2;
              //led_f2->fade(String(strtok(NULL, "_")).toInt());
          }else if (fadeLed =='3'){
            Serial.println("Ledf 1&2");
            led_id=3;
          }
          servo_speed=String(strtok(NULL, "_")).toInt();
          LedTask->setActive(true);
        } else if(device == "LEDB"){
          led_type=device;
          MsgServiceBT.sendMsg("Blink of an eye");
          Serial.println("Blink of an eye");
          if(String(strtok(NULL, "_"))=='1')
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
            Serial.print("Led ");
            Serial.print(led_id);
            Serial.println(" on");
            led_on = true;
          } else if (isOn=="OFF"){
            Serial.print("Led ");
            Serial.print(led_id);
            Serial.println(" off");
            led_on = false;
          } else if (isOn==NULL){
            led_on = !led_on;
          }
          LedTask->setActive(true);
        } else if(device == "IRRI"){
          Serial.println("servo");
          String val = String(strtok(NULL, "_"));
          if(val=="ON"){
            Serial.println("on");
            IrrigationTask->setActive(true);
          }else if(val=="OFF"){
            IrrigationTask->setActive(false);
          }else{
            servo_speed=val.toInt();
            IrrigationTask->init(1000-(val.toInt()*50));
          }
        }

        device = "";
        delete msg;
        this->state = CHECK_NEW_MESSAGE;
        break;

        default:
          Serial.println("DIFOULT");
          break;
    }
  }
