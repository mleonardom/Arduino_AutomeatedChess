#include "MotorController.h"

MotorController::MotorController(uint8_t dirPin, uint8_t stepPin) {
    _dirPin = dirPin;
    _stepPin = stepPin;
    pinMode(_dirPin, OUTPUT);
    pinMode(_stepPin, OUTPUT);
}

void MotorController::move(byte direction, int speed, float distance) {

    uint16_t step_number = 100;
    
    if (direction == R_L || direction == T_B) digitalWrite(_dirPin, HIGH);
    else digitalWrite(_dirPin, LOW);

    for (int x = 0; x < step_number; x++) {
        digitalWrite(_stepPin, HIGH);
        delayMicroseconds(speed);
        digitalWrite(_stepPin, LOW);
        delayMicroseconds(speed);
    }
  }