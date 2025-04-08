#include "BoardController.h"

BoardController::BoardController(
    uint8_t HButtonPin, 
    uint8_t VButtonPin, 
    uint8_t MUXAddr[], 
    uint8_t MUXOutputs[], 
    uint8_t HdirPin, 
    uint8_t HstepPin, 
    uint8_t VdirPin, 
    uint8_t VstepPin, 
    uint8_t electromagnetPin1,
    uint8_t electromagnetPin2) :
_MotorController(HButtonPin, VButtonPin, HdirPin, HstepPin, VdirPin, VstepPin),
_ElectromagnetController(electromagnetPin1, electromagnetPin2) {
    _MUXAddr = MUXAddr;
    _MUXOutputs = MUXOutputs;
}


void BoardController::setup() {
    for (byte i = 0; i < 4; i++) {
        pinMode (_MUXAddr[i], OUTPUT);
        digitalWrite(_MUXAddr[i], LOW);
    }

    for (byte i = 0; i < 4; i++) {
        // Disable pullup!
        /* SEE: https://www.arduino.cc/en/Tutorial/DigitalPins */
        digitalWrite(_MUXOutputs[i], LOW);
        pinMode (_MUXOutputs[i], INPUT);
    }
}

void BoardController::loop() {

}


void BoardController::readValues() {
    uint8_t row = 0;
    uint8_t column = 0;

    for (byte i = 0; i < 4; i++) {
        if( i < 2 ) column = 0;
        else column = 7;
        for (byte j = 0; j < 16; j++) {
            for (byte k = 0; k < 4; k++) {
                digitalWrite(_MUXAddr[k], _MUX_CHANNEL[j][k]);
            }
            _reedSensorRecord[row][column] = digitalRead(_MUXOutputs[i]);
            if( i < 2 ) column ++;
            else column --;
            if (j == 7) {
                row++;
                if( i < 2 ) column = 0;
                else column = 7;
            }
        }
        row ++;
    }
}

bool BoardController::squareHasPiece(uint8_t row, uint8_t column) {
    readValues();
    return _reedSensorRecord[row][column] == HIGH;
}

void BoardController::printSerial() {
    readValues();
    for(int i = 7; i >= 0; i --){
        for(int j = 7; j >= 0; j --){
            Serial.print(_reedSensorRecord[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
    Serial.println("------------------------------");
}

void BoardController::calibrate() {
    _MotorController.calibrate();
}

void BoardController::moveToString(String move) {
    int departure_coord_X = move[0] - 'a' + 1;
    int departure_coord_Y = move[1] - '0';
    int arrival_coord_X = move[2] - 'a' + 1;
    int arrival_coord_Y = move[3] - '0';

    Serial.print(move);
    Serial.print(": ");
    Serial.print(departure_coord_X);
    Serial.print(",");
    Serial.print(departure_coord_Y);
    Serial.print("->");
    Serial.print(arrival_coord_X);
    Serial.print(",");
    Serial.println(arrival_coord_Y);
    delay(3000);
}

String BoardController::calculateUserMovement() {
    String wMovementsDummies[8] = {
        "d2d4",
        "c2c3",
        "e2e3",
        "b2b3",
        "f2f3",
        "a2a3",
        "g2g3",
        "h2h3"
    };
    String bMovementsDummies[8] = {
        "d7d5",
        "c7c6",
        "e7e6",
        "b7b6",
        "f7f6",
        "a7a6",
        "g7g6",
        "h7h6"
    };

    if(currentDummyMovement >=8) {
        Serial.println("There are not more dummy movements!!!!");
        currentDummyMovement=0;
    }

    String userMove;
    if( _GameSettings.getHumanColor() == G_HUMAN_COLOR_WHITE ) {
        userMove = wMovementsDummies[currentDummyMovement];
    } else {
        userMove = bMovementsDummies[currentDummyMovement];
    }
    currentDummyMovement++;

    return userMove;
}

// Temporal
void BoardController::turnEM(uint8_t step) {
    if( step == 1 ) {
        Serial.println("Testing North");
        _ElectromagnetController.turnNorth();
    } else if( step == 2 ) {
        Serial.println("Testing South");
        _ElectromagnetController.turnSouth();
    } else {
        Serial.println("Testing OFF");
        _ElectromagnetController.turnOff();
    }
}