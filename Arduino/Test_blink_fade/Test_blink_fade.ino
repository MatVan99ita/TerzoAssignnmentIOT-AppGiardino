/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
int blink1 = 12;//BLINK
int blink2 = 11;//BLINK
int fade1 = 10;//FADE
int fade2 = 9;//FADE
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  
  Serial.begin(9600);
  pinMode(blink1, OUTPUT);
  pinMode(blink2, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(blink1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(blink2, HIGH);

  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) {
    // sets the value (range from 0 to 255):
    analogWrite(fade1, fadeValue);
    analogWrite(fade2, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }


  Serial.println("banana");
  delay(1000);
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 9) {
    // sets the value (range from 0 to 255):
    analogWrite(fade1, fadeValue);
    analogWrite(fade2, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
  digitalWrite(blink1, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(blink2, LOW);



 
  Serial.println("POMPELMO");  // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
