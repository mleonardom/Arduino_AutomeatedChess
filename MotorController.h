#ifndef MotorController_h
#define MotorController_h

#include "Arduino.h"
#include "Configs.h"

enum {T_B, B_T, L_R, R_L, LR_BT, RL_TB, LR_TB, RL_BT, CALIBRATION_DISTANCE};

class MotorController {
    private:
        uint8_t _HButtonPin, _VButtonPin, _HDirPin, _HStepPin, _VDirPin, _VStepPin;
        const float TROLLEY_START_POSITION_X = 2.78;
        const float TROLLEY_START_POSITION_Y = 4.65;
    public:
        MotorController(uint8_t HButtonPin, uint8_t VButtonPin, uint8_t HDirPin, uint8_t HStepPin, uint8_t VDirPin, uint8_t VStepPin);
        void calibrate();
        void move(byte direction, int speed, float distance);
};

#endif