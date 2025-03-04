#include <Arduino.h>

#include "MultiButtonsController.h"

MultiButtonsController::MultiButtonsController(int pin, int buttonCount, int arrVoltageRanges[][2], int adcMax) {
    _pin = pin;
    _adcMax = adcMax;
    _buttonCount = buttonCount;

    _arrVoltageRanges = new int* [buttonCount];
    for (int i = 0; i < buttonCount; i++) {
        _arrVoltageRanges[i] = arrVoltageRanges[i];
    }
}

void MultiButtonsController::setup() {

}

void MultiButtonsController::setPressCallback(std::function<void(uint)> pressCallback) {
    _pressCallback = pressCallback;
}

void MultiButtonsController::setReleaseCallback(std::function<void(uint, bool)> releaseCallback) {
    _releaseCallback = releaseCallback;
}

void MultiButtonsController::loop() {
    _curTime = millis();
    if (_curTime - _lastLoop >= _loopInterval) {
        _lastLoop = _curTime;
        _readButton();
    }
}

void MultiButtonsController::_readButton() {
    int _button = _getButton();

    if( _readPhase == BTN_READ_PHASE_1ST_PASS && _button != _lastPressedBtn ){
        _prevBtn = _button;
        _readPhase = BTN_READ_PHASE_2ND_PASS;
    } else if( _readPhase == BTN_READ_PHASE_2ND_PASS ) {
        if( _button == _prevBtn ) {
            if( _button == -1 && _releaseCallback != NULL && !_wasLongPress ) {
                _releaseCallback(_lastPressedBtn, false);
            } else if( _button > -1 ) {
                _pressTime = _curTime;
                _wasLongPress = false;
                if( _pressCallback != NULL ) {
                    _pressCallback(_button);
                }
            }
            _lastPressedBtn = _button;
        }
        _readPhase = BTN_READ_PHASE_1ST_PASS;
    } else if (  _button > -1 && _releaseCallback != NULL &&  _pressTime+LONG_PRESS_TIME <= _curTime ) {
        _pressTime = _curTime;
        _wasLongPress = true;
        _releaseCallback(_lastPressedBtn, true);
    }
}

int MultiButtonsController::_getButton() {
    int z, _sum;
    int _button = -1;

    _sum = 0;
    for (int i=0; i < 4; i++) {
        _sum += analogRead(_pin);
    }
    z = _sum / 4;

    if( z <= _adcMax ) {
        for (int i = 0; i < _buttonCount; i++) {
            if (z > _arrVoltageRanges[i][0] && z < _arrVoltageRanges[i][1]) {
                return i;
            }
        }
    }

    return -1;
}

bool MultiButtonsController::isButtonPressed(uint buttonId) {
    int _button = _getButton();
    return _button == buttonId;
}