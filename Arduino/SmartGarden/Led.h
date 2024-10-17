#ifndef __LED__
#define __LED__

class Led{
  public:
    Led(int pin);
    void switchOff();
    void switchOn();
    bool change();
    void fade(int val);
    void fadein(int val);
    void fadeout(int val);

  private:
    int pin;
    bool led_on;
};

#endif
