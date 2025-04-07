#include "ElectromagnetController.h"

ElectromagnetController::ElectromagnetController(uint8_t pin1, uint8_t pin2) {
    _pin1 = pin1;
    _pin2 = pin2;
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    turnOff();
}

void ElectromagnetController::turnNorth() {
    digitalWrite(_pin1, HIGH);
    digitalWrite(_pin2, LOW);
}

void ElectromagnetController::turnSouth() {
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, HIGH);
}

void ElectromagnetController::turnOff() {
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
}
