#ifndef BoardController_h
#define BoardController_h

#include "Arduino.h"

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
    public:
        BoardController(uint8_t MUXAddr[], uint8_t MUXOutputs[]);
        void setup();
        void loop();
        void readValues();
        bool squareHasPiece(uint8_t row, uint8_t column);
        void printSerial();

};

#endif