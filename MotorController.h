#ifndef MotorController_h
#define MotorController_h

#include "Arduino.h"
#include "Configs.h"

enum {T_B, B_T, L_R, R_L, LR_BT, RL_TB, LR_TB, RL_BT};

class MotorController {
    private:
        uint8_t _dirPin, _stepPin;
    public:
        MotorController(uint8_t dirPin, uint8_t step_pin);
        void move(byte direction, int speed, float distance);
};

#endif