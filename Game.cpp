#include "Game.h"

Game::Game() {
}

void Game::loop() {
    if( _gameState == GAME_PLAYING_STATUS ) {
        bool notify = false;
        if( WhiteClock.loop() ) {
            notify = true;
        }
        if( BlackClock.loop() ) {
            notify = true;
        }

        if( notify ) _gameChangedCallback();
    }
}

uint8_t Game::getGameState() {
    return _gameState;
}

void Game::setAIServer(String AIServer) {
    _RestController.setAIServer(AIServer);
}

void Game::restart() {
    if( isInGame() ) {
        _cleanMoves();
        WhiteClock.stop();
        BlackClock.stop();
    }
    _gameState = GAME_UNSET_SATUS;
}

void Game::_addMove(String move, uint8_t index) {
    if( _movesInHistory < 5 ) {
        if( index==0 ) _movesInHistory++;
        _lastMoves[_movesInHistory-1][index] = move;
    } else {
        if( index == 0 ) {
            String temp[5][2];
            for( uint8_t i=0; i<5; i++) {
                for( uint8_t j=0; j<2; j++) {
                    temp[i][j] = _lastMoves[i][j];
                }
            }
            for( uint8_t i=0; i<4; i++) {
                for( uint8_t j=0; j<2; j++) {
                    _lastMoves[i][j] = _lastMoves[i+1][j];
                }
            }
            _lastMoves[4][1] = "";
        }
        _lastMoves[4][index] = move;
    }
}

void Game::_cleanMoves() {
    _movesInHistory = 0;
    for( uint8_t i=0; i<5; i++) {
        for( uint8_t j=0; j<2; j++) {
            _lastMoves[i][j] = "";
        }
    }
}

void Game::setGameChangedCallback(std::function<void()> gameChangedCallback) {
    _gameChangedCallback = gameChangedCallback;
}

void Game::setWhiteTimeoutCallback(std::function<void()> whiteTimeoutCallback) {
    WhiteClock.setTimeoutCallback(whiteTimeoutCallback);
}

void Game::setBlackTimeoutCallback(std::function<void()> blackTimeoutCallback) {
    BlackClock.setTimeoutCallback(blackTimeoutCallback);
}

void Game::setAIMoveCallback(std::function<void(String)> AImoveCallback) {
    _AImoveCallback = AImoveCallback;
}

bool Game::isInGame() {
    return _gameState == GAME_PLAYING_STATUS || 
    _gameState == GAME_WAITING_CLOCK_STATUS || 
    _gameState == GAME_WAITING_USER_MOVE ||
    _gameState == GAME_AI_THINKING;
}

bool Game::isGameFinished() {
    return _gameState == GAME_FINISHED_STATUS;
}

void Game::pause() {
    _gameState = GAME_PAUSED_STATUS;
    if( isInGame() ) {
        WhiteClock.pause();
        BlackClock.pause();
    }
    _gameChangedCallback();
}

void Game::start() {
    _gameState = hasClocks()? GAME_WAITING_CLOCK_STATUS : GAME_PLAYING_STATUS;
    if( _GameSettings.getGameMode() == G_MODE_H_C ) {
        _cleanMoves();
        _gameState = GAME_AI_LOADING;
        _gameChangedCallback();
        String move = _RestController.initGame();
        if( move != NULL && move != "" ) {
            _addMove(move, 0);
            _gameState = GAME_AI_THINKING; //AI is not really thinking but moving the piece
            _gameChangedCallback();
            _AImoveCallback(move);
        }
        _gameState = GAME_WAITING_USER_MOVE;
    }
    _gameChangedCallback();
}

void Game::gameOver() {
    if( isInGame() ) {
        WhiteClock.stop();
        BlackClock.stop();
    }
    _gameState = GAME_FINISHED_STATUS;
    _gameChangedCallback();
}

void Game::startWhiteClock() {
    if( !isInGame() || !hasClocks() || WhiteClock.isRunning() ) return;
    if( _gameState == GAME_WAITING_CLOCK_STATUS ) {
        _gameState = GAME_PLAYING_STATUS;
    } else {
        BlackClock.pause();
    }
    WhiteClock.start();
    _gameChangedCallback();
}

void Game::startBlackClock() {
    if( _gameState == GAME_WAITING_CLOCK_STATUS || !isInGame() || !hasClocks()  || BlackClock.isRunning()) return;
    WhiteClock.pause();
    BlackClock.start();
    _gameChangedCallback();
}

bool Game::hasClocks() {
    return WhiteClock.getInitialMinutes()!=0;
}

String* Game::getLastMoves(uint8_t index) {
    return _lastMoves[index];
}

uint8_t Game::getCountMovesInHistory() {
    return _movesInHistory;
}

bool Game::putUserMovement(String move) {
    if( _gameState != GAME_WAITING_USER_MOVE ) return false;

    if( _GameSettings.getHumanColor() == G_HUMAN_COLOR_WHITE ) {
        _addMove(move, 0);
    } else {
        _addMove(move, 1);
    }
    _gameState = GAME_AI_THINKING;
    _gameChangedCallback();
    String aiMove = _RestController.userMove(move);
    if( aiMove != NULL && aiMove != "" ) {
        _isValidMovement = true;
        if( _GameSettings.getHumanColor() == G_HUMAN_COLOR_WHITE ) {
            _addMove(aiMove, 1);
        } else {
            _addMove(aiMove, 0);
        }
        _gameChangedCallback();
        _AImoveCallback(aiMove);
        _gameState = GAME_WAITING_USER_MOVE;
        _gameChangedCallback();

        return true;
    }
    if( _GameSettings.getHumanColor() == G_HUMAN_COLOR_WHITE ) {
        _addMove("Inval", 1);
    } else {
        _addMove("Inval", 0);
    }
    _isValidMovement = false;
    _gameState = GAME_WAITING_USER_MOVE;
    _gameChangedCallback();

    return false;
}