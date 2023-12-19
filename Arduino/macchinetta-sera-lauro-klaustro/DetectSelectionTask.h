#ifndef __DETECTSELECTIONTASK__
#define __DETECTSELECTIONTASK__

#include "Task.h"
#include "Button.h"


class DetectSelectionTask: public Task {
    /** bottoni */
    Button* buttonUp;
    Button* buttonDown;
    Button* buttonMake;
    /** pin */
    int pinUp, pinDown, pinMake;
    /** stati che compongono il task */
    enum {_PRESSED, NOT_PRESSED} states;
    /** stati dei bottoni */
    bool stateButtonUp;
    bool stateButtonDown;
    bool stateButtonMake;

    public:
        DetectSelectionTask(int pinUp, int pinDown ,int pinMake);
        void init(int period);  
        void tick();
};
#endif
