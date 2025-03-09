#include "BoardController.h"

BoardController::BoardController(uint8_t MUXAddr[], uint8_t MUXOutputs[]) {
    _MUXAddr = MUXAddr;
    _MUXOutputs = MUXOutputs;
}

void BoardController::setup() {
    for (byte i = 0; i < 4; i++) {
        Serial.print("Pinout: ");
        Serial.println(_MUXAddr[i]);
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
    uint8_t column = 0;
    uint8_t row = 0;

    for (byte i = 0; i < 4; i++) {
        for (byte j = 0; j < 16; j++) {
            for (byte k = 0; k < 4; k++) {
                digitalWrite(_MUXAddr[k], _MUX_CHANNEL[j][k]);
            }
            _reedSensorRecord[column][row] = digitalRead(_MUXOutputs[i]);
            row++;
            if (j == 7) {
                column++;
                row = 0;
            }
        }
        column ++;
        row = 0;
    }
}

bool BoardController::squareHasPiece(uint8_t row, uint8_t column) {
    readValues();
    return _reedSensorRecord[row][column] == HIGH;
}

void BoardController::printSerial() {
    readValues();
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            Serial.print(_reedSensorRecord[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
    Serial.println("------------------------------");
}