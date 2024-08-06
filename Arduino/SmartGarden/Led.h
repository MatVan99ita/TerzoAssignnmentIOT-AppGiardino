#ifndef __LED__
#define __LED__

class Led{
  public:
    Led(int pin);
    void switchOff();
    void switchOn();
    bool change();
    void fade(int val);

  private:
  int pin;
};

#endif
