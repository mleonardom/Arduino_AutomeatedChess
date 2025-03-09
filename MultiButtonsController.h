#ifndef MultiButtonsController_h
#define MultiButtonsController_h

#include <Arduino.h>

#include "Configs.h"

enum {BTN_READ_PHASE_1ST_PASS, BTN_READ_PHASE_2ND_PASS};
// #define BTN_READ_PHASE_1ST_PASS  0
// #define BTN_READ_PHASE_2ND_PASS  1

class MultiButtonsController {
    private:
        int _pin;
        int **_arrVoltageRanges;
        int _adcMax;
        int _buttonCount;
        unsigned long _curTime;
        unsigned long _lastLoop = 0;
        unsigned long _pressTime = 0;
        int _loopInterval = 50;
        int _prevBtn = -1;
        int _lastPressedBtn = -1;
        int _readPhase = BTN_READ_PHASE_1ST_PASS;
        bool _wasLongPress = false;

        std::function<void(uint)> _pressCallback;
        std::function<void(uint, bool)> _releaseCallback;

        void _readButton();
        int _getButton();
    public:
        MultiButtonsController(int pin, int buttonCount, int arrVoltageRanges[][2], int adcMax);
        void setup();
        void loop();
        bool isButtonPressed(uint buttonId);
        void setPressCallback(std::function<void(uint)> pressCallback);
        void setReleaseCallback(std::function<void(uint, bool)> releaseCallback);
};

#endif