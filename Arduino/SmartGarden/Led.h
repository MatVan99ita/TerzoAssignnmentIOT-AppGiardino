#ifndef __LED__
#define __LED__

class Led{
  public:
    Led(int pin);
    void switchOff();
    void switchOn();
    bool change();
    void fade();
    int fadeIn(int v);
    int fadeOut(int v);

  private:
  int pin;
};

#endif