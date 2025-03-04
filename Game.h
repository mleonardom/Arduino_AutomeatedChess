#ifndef Game_h
#define Game_h

#include "Arduino.h"

#include "GameSettings.h"
#include "Menu.h"
#include "ChessClock.h"

#define GAME_UNSET_SATUS            0
#define GAME_PLAYING_STATUS         1
#define GAME_PAUSED_STATUS          2
#define GAME_WAITING_CLOCK_STATUS   3
#define GAME_FINISHED_STATUS        4

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