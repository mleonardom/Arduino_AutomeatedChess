#ifndef MotorController_h
#define MotorController_h

#include "Arduino.h"
#include "Configs.h"

enum {T_B, B_T, L_R, R_L, LR_BT, RL_TB, LR_TB, RL_BT};

class MotorController {
    private:
        uint8_t _HdirPin, _HstepPin, _VdirPin, _VstepPin;
    public:
        MotorController(uint8_t HdirPin, uint8_t HstepPin, uint8_t VdirPin, uint8_t VstepPin);
        void move(byte direction, int speed, float distance);
};

#endif