#include "Button.h"
#include "Arduino.h"
#include "Config.h"

Button::Button(int pin){
    this->pin = pin;
    pinMode(pin, INPUT);
}

/** return true if the button is being pressed */
bool Button::isPressed(){
    return digitalRead(this -> pin) == HIGH;
}
