#ifndef ElectromagnetController_h
#define ElectromagnetController_h

#include "Arduino.h"

class ElectromagnetController {
    private:
        uint8_t _pin1, _pin2;
    public:
        ElectromagnetController(uint8_t pin1, uint8_t pin2);
        void turnNorth();
        void turnSouth();
        void turnOff();
};

#endif