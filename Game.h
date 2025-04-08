#ifndef Game_h
#define Game_h

#include "Arduino.h"

#include "GameSettings.h"
#include "Menu.h"
#include "ChessClock.h"
#include "RestController.h"

enum {GAME_UNSET_SATUS, GAME_PLAYING_STATUS, GAME_PAUSED_STATUS, GAME_WAITING_CLOCK_STATUS, GAME_FINISHED_STATUS, GAME_WAITING_USER_MOVE, GAME_AI_THINKING, GAME_AI_LOADING};

class Game {

    private:
        uint8_t _gameState = GAME_UNSET_SATUS;

        std::function<void()> _gameChangedCallback;
        std::function<void(String)> _AImoveCallback;

        uint8_t _movesInHistory;
        bool _isValidMovement = true;
        String _lastMoves[5][2] = {
            {"",""},
            {"",""},
            {"",""},
            {"",""},
            {"",""}
        };

        RestController _RestController;
        void _addMove(String move, uint8_t index);
        void _cleanMoves();
    public:
        Game();
        void loop();
        bool isInGame();
        bool isGameFinished();
        void setGameChangedCallback(std::function<void()> gameChangedCallback);
        void setWhiteTimeoutCallback(std::function<void()> whiteTimeoutCallback);
        void setBlackTimeoutCallback(std::function<void()> blackTimeoutCallback);
        void setAIMoveCallback(std::function<void(String)> AImoveCallback);
        uint8_t getGameState();
        void setAIServer(String AIServer);
        void restart();
        void pause();
        void start();
        void gameOver();
        bool hasClocks();
        void startWhiteClock();
        void startBlackClock();
        uint8_t getCountMovesInHistory();
        String* getLastMoves(uint8_t index);
        bool putUserMovement(String move);

};

#endif