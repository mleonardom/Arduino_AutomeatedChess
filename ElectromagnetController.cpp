#include "ElectromagnetController.h"

ElectromagnetController::ElectromagnetController(uint8_t pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
}

void ElectromagnetController::turnOn() {
    _outputLevel = HIGH;
    digitalWrite(_pin, _outputLevel);
}

void ElectromagnetController::turnOff() {
    _outputLevel = LOW;
    digitalWrite(_pin, _outputLevel);
}

bool ElectromagnetController::isTurnedOn() {
    return _outputLevel == HIGH;
}
