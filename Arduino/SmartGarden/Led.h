#ifndef __LED__
#define __LED__

class Led{
  public:
    Led(int pin);
    void switchOff();
    void switchOn();
    bool change();
    void fadeIn(int v);
    void fadeOut(int v);

  private:
  int pin;
};

#endif