#ifndef ELOSetter_h
#define ELOSetter_h

#include "Arduino.h"

class ELOSetter {
    private:
        uint16_t _eloStep = 50;
        uint16_t _minElo = 1350;
        uint16_t _maxElo = 2000;
        uint16_t _selectedElo = _minElo;
    public:
        ELOSetter();
        void prevButton(bool isLongPress);
        void nextButton(bool isLongPress);
        uint16_t getSelectedElo();
};

#endif