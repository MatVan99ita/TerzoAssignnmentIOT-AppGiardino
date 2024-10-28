#ifndef __LED__
#define __LED__

class Led{
  public:
    Led(int pin);
    bool change();
    void fade(int val);

  private:
    int pin;
    bool led_on;
};

#endif
