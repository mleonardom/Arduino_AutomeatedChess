#include "BoardController.h"

BoardController::BoardController(uint8_t MUXAddr[], uint8_t MUXOutputs[], uint8_t MUXOutput) {
    _MUXAddr = MUXAddr;
    _MUXOutputs = MUXOutputs;
    //_MUXOutput = MUXOutput;
}

void BoardController::setup() {
    for (byte i = 0; i < 4; i++) {
        pinMode (_MUXAddr[i], OUTPUT);
        digitalWrite(_MUXAddr[i], LOW);
    }

    for (byte i = 0; i < 4; i++) {
        digitalWrite(_MUXOutputs[i], LOW);
        pinMode (_MUXOutputs[i], INPUT);
    }
    
    // Disable pullup!
    /* SEE: https://www.arduino.cc/en/Tutorial/DigitalPins */
    // digitalWrite(_MUXOutput, LOW);
    // pinMode (_MUXOutput, INPUT);
}

void BoardController::loop() {
    byte column = 6;
    byte row = 0;
    
    // for (int i = 0; i < 4; i++) {
    //     for (int j = 0; j < 16; j++) {
    //         for(int k = 0; k < 4; k ++){
    //             Serial.print(_MUX_CHANNEL[j][k]);
    //             digitalWrite(_MUXAddr[k], _MUX_CHANNEL[j][k]);
    //         }
    //         Serial.println();
    //         _reedSensorRecord[i][j] = digitalRead(_MUXOutputs[i]);
    //     }
    // }

    for (byte i = 0; i < 4; i++) {
        // digitalWrite(_MUXSelect[i], LOW);
        for (byte j = 0; j < 16; j++) {
            for (byte k = 0; k < 4; k++) {
                digitalWrite(_MUXAddr[k], _MUX_CHANNEL[j][k]);
            }
            _reedSensorRecord[column][row] = digitalRead(_MUXOutputs[i]);
            //_F[column][row] = analogRead(_MUXOutput);
            row++;
            if (j == 7) {
                column++;
                row = 0;
            }
        }
        // for (byte l = 0; l < 4; l++) {
        //     digitalWrite(_MUXSelect[l], HIGH);
        // }
        if (i == 0) column = 4;
        if (i == 1) column = 2;
        if (i == 2) column = 0;
        row = 0;
    }
    // for (byte i = 0; i < 8; i++) {
    //     Serial.print(i);
    //     Serial.print(": ");
    //     for (byte j = 0; j < 8; j++) {
    //         Serial.print(_reedSensorRecord[i][j]);
    //         if(j<7) Serial.print("-");
    //     }
    //     Serial.println(";");
    // }
    // Serial.println("-----------------------------");
    // delay(1000);
}

bool BoardController::squareHasPiece(uint8_t row, uint8_t column) {
    loop();
    return _reedSensorRecord[row][column] == HIGH;
}

void BoardController::printSerial() {
    loop();
    for(int i = 0; i < 8; i ++){
        for(int j = 0; j < 8; j ++){
            Serial.print(_reedSensorRecord[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }
    Serial.println("------------------------------");
}