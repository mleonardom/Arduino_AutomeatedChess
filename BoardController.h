#ifndef BoardController_h
#define BoardController_h

#include "Arduino.h"

#include "MotorController.h"
#include "ElectromagnetController.h"

class BoardController {

    private:
        uint8_t *_MUXAddr;
        uint8_t *_MUXOutputs;
        uint8_t _reedSensorRecord[8][8];

        const byte _MUX_CHANNEL[16][4] = {
            {0, 0, 0, 0},
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {1, 1, 0, 0},
            {0, 0, 1, 0},
            {1, 0, 1, 0},
            {0, 1, 1, 0},
            {1, 1, 1, 0},
            {0, 0, 0, 1},
            {1, 0, 0, 1},
            {0, 1, 0, 1},
            {1, 1, 0, 1},
            {0, 0, 1, 1},
            {1, 0, 1, 1},
            {0, 1, 1, 1},
            {1, 1, 1, 1}
        };

        MotorController _MotorController;
        ElectromagnetController _ElectromagnetController;
    public:
        BoardController(uint8_t HButtonPin, 
            uint8_t VButtonPin, 
            uint8_t MUXAddr[], 
            uint8_t MUXOutputs[], 
            uint8_t HdirPin, 
            uint8_t HstepPin, 
            uint8_t VdirPin, 
            uint8_t VstepPin, 
            uint8_t electromagnetPin);
        void setup();
        void readValues();
        bool squareHasPiece(uint8_t row, uint8_t column);
        void printSerial();
        void calibrate();
        void moveToString(String move);
        // Temporal
        void turnEM(bool isOn);

};

#endif