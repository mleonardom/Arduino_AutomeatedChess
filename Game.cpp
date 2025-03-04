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

void Game::restart() {
    if( isInGame() ) {
        WhiteClock.stop();
        BlackClock.stop();
    }
    _gameState = GAME_UNSET_SATUS;
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

bool Game::isInGame() {
    return _gameState == GAME_PLAYING_STATUS || _gameState == GAME_WAITING_CLOCK_STATUS;
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