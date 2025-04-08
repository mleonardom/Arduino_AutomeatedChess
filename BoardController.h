#ifndef BoardController_h
#define BoardController_h

#include "Arduino.h"

#include "MotorController.h"
#include "ElectromagnetController.h"
#include "GameSettings.h"

class BoardController {

    private:
        uint8_t *_MUXAddr;
        uint8_t *_MUXOutputs;
        uint8_t _reedSensorRecord[8][8];
        uint8_t _reedSensorStausMemory[8][8];

        //TODO Deleteme
        uint8_t currentDummyMovement = 0;

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

        std::function<void(String)> _humanMoveCallback;
    public:
        BoardController(uint8_t HButtonPin, 
            uint8_t VButtonPin, 
            uint8_t MUXAddr[], 
            uint8_t MUXOutputs[], 
            uint8_t HdirPin, 
            uint8_t HstepPin, 
            uint8_t VdirPin, 
            uint8_t VstepPin, 
            uint8_t electromagnetPin1,
            uint8_t electromagnetPin2);
        void setup();
        void loop();
        void readValues();
        bool squareHasPiece(uint8_t row, uint8_t column);
        void printSerial();
        void calibrate();
        void moveToString(String move);
        String calculateUserMovement();
        // Temporal
        void turnEM(uint8_t step);

};

#endif