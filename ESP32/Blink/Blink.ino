/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
int pin = 26;//sul d2
int angle = 0;
int lightValue;
double tempValue = 0.0;
void setup() {
  Serial.begin(115200);
  pinMode(pin, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

}

// the loop function runs over and over again forever
void loop() {
  Serial.println("si");
  digitalWrite(pin, HIGH);
  lightValue = analogRead(4); // read analog input pin 0
  tempValue = analogRead(5); // read analog input pin 0
  angle = map(tempValue, 0, 4095, 0, 10);

  //Calculate Temperature from ADC value
  //Note that we use mV for Vref
  //Vin = ADCresult*Vref/(2^10)
  //Temp(C) = Vin/(10) = ADCresult*Vref/(1024*10)
  //tempValue = tempValue*1100/(1024*10);
   // convert the analog volt to its temperature equivalent
  Serial.print("TEMPERATURE = ");
  Serial.print(angle); // display temperature value
  Serial.print("*C");
  Serial.println();
  Serial.print("Luce = ");
  Serial.print(lightValue, DEC); // prints the value read
  Serial.print(" \n");
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
                    // Wait for a second
  digitalWrite(pin, LOW);           // Wait for two seconds (to demonstrate the active low LED)
}
