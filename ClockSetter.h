#ifndef ClockSetter_h
#define ClockSetter_h

#include "Arduino.h"

class ClockSetter {

    private:
        uint8_t _setStep = 0;

        uint8_t _maxMinutes = 60;
        uint8_t _maxIncrement = 60;

        uint8_t _wMinutes = 0;
        uint8_t _wIncrement = 0;

        uint8_t _bMinutes = 0;
        uint8_t _bIncrement = 0;

        String getStringForNumber(uint8_t number);
    public:
        ClockSetter();
        void prevButton(bool isLongPress);
        void nextButton(bool isLongPress);
        void selectButton(bool isLongPress);

        uint8_t getSetStep();

        uint8_t getWMinutes();
        uint8_t getWIncrement();
        uint8_t getBMinutes();
        uint8_t getBIncrement();

        String getWMinutesString();
        String getWIncrementString();
        String getBMinutesString();
        String getBIncrementString();

        bool isDone();

};

#endif