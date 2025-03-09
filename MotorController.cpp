#include "MotorController.h"

MotorController::MotorController(uint8_t HdirPin, uint8_t HstepPin, uint8_t VdirPin, uint8_t VstepPin) {
    _HdirPin = HdirPin;
    _HstepPin = HstepPin;
    _VdirPin = VdirPin;
    _VstepPin = VstepPin;
    pinMode(_HdirPin, OUTPUT);
    pinMode(_HstepPin, OUTPUT);
    pinMode(_VdirPin, OUTPUT);
    pinMode(_VstepPin, OUTPUT);
}

void MotorController::move(byte direction, int speed, float distance) {

    uint16_t step_number = 100;
    
    if (direction == R_L || direction == T_B) digitalWrite(_HdirPin, HIGH);
    else digitalWrite(_HdirPin, LOW);

    for (int x = 0; x < step_number; x++) {
        digitalWrite(_HstepPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(_HstepPin, LOW);
        delayMicroseconds(speed);
    }
  }