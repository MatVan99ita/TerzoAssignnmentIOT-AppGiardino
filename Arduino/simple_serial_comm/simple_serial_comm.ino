String x;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() {
  while (!Serial.available()){
    x = Serial.readString();//.toInt();
    Serial.print("MESSAGGIO: ");
    Serial.println(x);
    //Serial.println("irrigazione:PAUSA,mode:AUTO");
  }
}
