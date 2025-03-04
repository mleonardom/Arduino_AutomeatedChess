#ifndef ElectromagnetController_h
#define ElectromagnetController_h

#include "Arduino.h"

class ElectromagnetController {
    private:
        uint8_t _pin;
        uint8_t _outputLevel;
    public:
        ElectromagnetController(uint8_t pin);
        void turnOn();
        void turnOff();
        bool isTurnedOn();
};

#endif