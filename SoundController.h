#ifndef SoundController_h
#define SoundController_h

#include "Arduino.h"
#include "DFPlayerMini_Fast.h"

#include "Configs.h"

#define FPSerial Serial1

#define START_SOUND_INDEX                   1
#define ERROR_SOUND_INDEX                   2
#define CHECKMATE_SOUND_INDEX               3
#define PIECE_SOUND_INDEX                   4
#define NOTIFICVATION1_SOUND_INDEX          5
#define LOW_SIGNAL_SOUND_INDEX              6
#define TIMEOUT_SOUND_INDEX                 7
#define CAPTURE_SOUND_INDEX                 8
#define BOARD_START_SOUND_INDEX             9
#define SELECT_UI_SOUND_INDEX               10
#define PIECE_2_SOUND_INDEX                 11

class SoundController {

    private:
        DFPlayerMini_Fast _DFPlayer;
        int startSound = 1;

    public:
        SoundController();
        void setup();
        void play(uint16_t trackNum);
        void playStartSound();
        void playErrorSound();
        void playCheckmateSound();
        void playPieceSound();
        void playNotification1Sound();
        void playLowSignalSound();
        void playTimeoutSound();
        void playSelectUISound();

};

#endif