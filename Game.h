#ifndef Game_h
#define Game_h

#include "Arduino.h"

#include "GameSettings.h"
#include "Menu.h"
#include "ChessClock.h"

enum {GAME_UNSET_SATUS, GAME_PLAYING_STATUS, GAME_PAUSED_STATUS, GAME_WAITING_CLOCK_STATUS, GAME_FINISHED_STATUS};

class Game {

    private:
        uint8_t _gameState = GAME_UNSET_SATUS;

        std::function<void()> _gameChangedCallback;
    public:
        Game();
        void loop();
        bool isInGame();
        bool isGameFinished();
        void setGameChangedCallback(std::function<void()> gameChangedCallback);
        void setWhiteTimeoutCallback(std::function<void()> whiteTimeoutCallback);
        void setBlackTimeoutCallback(std::function<void()> blackTimeoutCallback);
        uint8_t getGameState();
        void restart();
        void pause();
        void start();
        void gameOver();
        bool hasClocks();
        void startWhiteClock();
        void startBlackClock();

};

#endif