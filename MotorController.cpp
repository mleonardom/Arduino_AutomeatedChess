#include "MotorController.h"

MotorController::MotorController(uint8_t HButtonPin, uint8_t VButtonPin, uint8_t HDirPin, uint8_t HStepPin, uint8_t VDirPin, uint8_t VStepPin) {
    _HButtonPin = HButtonPin;
    _VButtonPin = VButtonPin;
    _HDirPin = HDirPin;
    _HStepPin = HStepPin;
    _VDirPin = VDirPin;
    _VStepPin = VStepPin;
    pinMode(_HDirPin, OUTPUT);
    pinMode(_HStepPin, OUTPUT);
    pinMode(_VDirPin, OUTPUT);
    pinMode(_VStepPin, OUTPUT);
}

void MotorController::calibrate() {
    while (digitalRead(_HButtonPin) == LOW) {
        move(R_L, SPEED_SLOW, CALIBRATION_DISTANCE);
    }
    Serial.println("Calibrated H");
    while (digitalRead(_VButtonPin) == LOW) {
        move(B_T, SPEED_SLOW, CALIBRATION_DISTANCE);
    }
    Serial.println("Calibrated V");

    //  Rapid displacements up to the Black start position (e7)
    move(L_R, SPEED_FAST, TROLLEY_START_POSITION_X);
    move(T_B, SPEED_FAST, TROLLEY_START_POSITION_Y);
    delay(500);
}

void MotorController::move(byte direction, int speed, float distance) {
    float step_number = 0;

    //  Calcul the distance
    if (distance == CALIBRATION_DISTANCE) step_number = 4;
    else if (direction == LR_BT || direction == RL_TB || direction == LR_TB || direction == RL_BT) step_number = distance * SQUARE_SIZE * 1.44; //  Add an extra length for the diagonal
    else step_number = distance * SQUARE_SIZE;

    //  Direction of the motor rotation
    if (direction == T_B || direction == R_L || direction == RL_TB) digitalWrite(_HDirPin, HIGH);
    else digitalWrite(_HDirPin, LOW);
    if (direction == T_B || direction == L_R || direction == LR_TB) digitalWrite(_VDirPin, HIGH);
    else digitalWrite(_VDirPin, LOW);

    //  Active the motors
    for (int x = 0; x < step_number; x++) {

        if( direction != LR_TB && direction != RL_BT ) {
            digitalWrite(_HStepPin, HIGH);
        }
        if( direction != LR_BT && direction != RL_TB ) {
            digitalWrite(_VStepPin, HIGH);
        }
        delayMicroseconds(speed);
        digitalWrite(_HStepPin, LOW);
        digitalWrite(_VStepPin, LOW);
        delayMicroseconds(speed);
    }
}