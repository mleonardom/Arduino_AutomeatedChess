#include "ChessClock.h"

ChessClock WhiteClock;
ChessClock BlackClock;

ChessClock::ChessClock() {
    _isSetted = false;
}

void ChessClock::setTimeoutCallback(std::function<void()> timeoutCallback) {
    _timeoutCallback = timeoutCallback;
}

bool ChessClock::loop() {
    if( _remainingTime==1 && _CD.isStopped() ) {
        _remainingTime = 0;
        _timeoutCallback();
        return true;
    }
    if( _CD.isStopped() || _CD.remaining() == _remainingTime ) {
        return false;
    }
    _remainingTime = _CD.remaining();
    return true;
}

void ChessClock::setValues(String str) {
    uint8_t plusPosition = str.indexOf("+");
    uint8_t initialMinutes = str.substring(0, plusPosition).toInt();
    uint8_t increment = str.substring(plusPosition+1, str.length()).toInt();
    setValues(initialMinutes, increment);
}

void ChessClock::setValues(uint8_t minutes, uint8_t increment) {
    _CD.setResolution(CountDown::SECONDS);
    _initialMinutes = minutes;
    _increment = increment;
    _remainingTime = (_initialMinutes*60);
    _isSetted = true;
}

String ChessClock::getStringForNumber(uint8_t number) {
    return number < 10? "0"+((String)number) : (String)number;
}

void ChessClock::unset() {
    _initialMinutes = 0;
    _increment = 0;
    _remainingTime = 0;
    _isSetted = false;
    isFirstTurn = true;
    _moves = 0;
}

void ChessClock::start() {
    if( isFirstTurn ) {
        _CD.start(_remainingTime);
        isFirstTurn = false;
    } else {
        _CD.start(_remainingTime+_increment);
    }
}

void ChessClock::pause() {
    _remainingTime = _CD.remaining();
    _CD.stop();
    _moves ++;
}

void ChessClock::stop() {
    _CD.stop();
}

bool ChessClock::isSetted() {
    return _isSetted;
}

uint8_t ChessClock::getIncrement() {
    return _increment;
}

uint8_t ChessClock::getInitialMinutes() {
    return _initialMinutes;
}

String ChessClock::getIncrementString() {
    return getStringForNumber(_increment);
}

String ChessClock::getInitialMinutesString() {
    return getStringForNumber(_initialMinutes);
}

uint16_t ChessClock::getMoves() {
    return _moves;
}

String ChessClock::getRemainingMinutesString() {
    uint8_t remainingMinutes = _remainingTime/60;

    return getStringForNumber(remainingMinutes);
}

String ChessClock::getRemainingSecondsString() {
    uint8_t remainingSeconds = _remainingTime%60;

    return getStringForNumber(remainingSeconds);
}

bool ChessClock::isRunning() {
    return _CD.isRunning();
}