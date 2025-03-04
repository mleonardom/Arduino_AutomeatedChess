#include "GameSettings.h"

GameSettings::GameSettings() {
    _currentMenu = Menu(MENU_MODE_ID, _modeTitle, ARRAYSIZE(_modeItems), _modeItems);
}

bool GameSettings::hasSettings() {
    return ( _gameMode!=0 && (_gameMode!=G_MODE_H_C || _humanColor!=0) );
}

bool GameSettings::isClocksSetted() {
    return BlackClock.isSetted() && WhiteClock.isSetted();
}

Menu GameSettings::getMenu() {
    return _currentMenu;
}

ClockSetter GameSettings::getClockSetter() {
    return _clockSetter;
}

ELOSetter GameSettings::getELOSetter() {
    return _ELOSetter;
}

void GameSettings::setParamsSettedCallback(std::function<void()> paramsSettedCallback) {
    _paramsSettedCallback = paramsSettedCallback;
}

void GameSettings::setMenuChangedCallback(std::function<void()> menuChangedCallback) {
    _menuChangedCallback = menuChangedCallback;
}

void GameSettings::setRestartGameCallback(std::function<void()> restartGameCallback) {
    _restartGameCallback = restartGameCallback;
}

void GameSettings::prevButton(bool isLongPress) {
    if( _currentMenu.getID() == 0 ) {
        if( isClocksSetted() ) {
            _ELOSetter.prevButton(isLongPress);
        } else {
            _clockSetter.prevButton(isLongPress);
        }
    } else {
        _currentMenu.selectPrevItem();
    }
    _menuChangedCallback();
}

void GameSettings::nextButton(bool isLongPress) {
    if( _currentMenu.getID() == 0 ) {
        if( isClocksSetted() ) {
            _ELOSetter.nextButton(isLongPress);
        } else {
            _clockSetter.nextButton(isLongPress);
        }
    } else {
        _currentMenu.selectNextItem();
    }
    _menuChangedCallback();
}

void GameSettings::selectButton(bool isLongPress) {
    if( _currentMenu.getID() == 0 ) {
        if( isClocksSetted() ) {
            _setAIELO();
        } else {
            _setCustomClock(isLongPress);
        }
    } else if( _currentMenu.getID() == MENU_MODE_ID ) {
        _setMode();
    } else if( _currentMenu.getID() == MENU_HUMAN_COLOR_ID ) {
        _setHumanColor();
    } else if( _currentMenu.getID() == MENU_CLOCK_ID ) {
        _setClock();
    } else if( _currentMenu.getID() == MENU_RESTART_GAME_ID ) {
        if( _currentMenu.getSelectedIndex() == 1 ) {
            _restartSettings();
        } else {
            _paramsSettedCallback();
        }
    }
}

uint8_t GameSettings::getGameMode() {
    return _gameMode;
}

uint8_t GameSettings::getHumanColor() {
    return _humanColor;
}

uint16_t GameSettings::getAIELO() {
    return _aiELO;
}

void GameSettings::_restartSettings() {
    restart();
    _restartGameCallback();
}

void GameSettings::_setAIELO() {
    _aiELO = _ELOSetter.getSelectedElo();
    _finishConfigurations();
}

void GameSettings::_setMode() {
    _gameMode = _currentMenu.getSelectedIndex()+1;
    if( _gameMode == G_MODE_H_C ) {
        _currentMenu = Menu(MENU_HUMAN_COLOR_ID, _humanColorTitle, ARRAYSIZE(_humanColorItems), _humanColorItems);
    } else {
        _currentMenu = Menu(MENU_CLOCK_ID, _clockTitle, ARRAYSIZE(_clockItems), _clockItems);
    }
    _menuChangedCallback();
}

void GameSettings::_setHumanColor() {
    _humanColor = _currentMenu.getSelectedIndex()+1;
    if( AI_USE_CLOCKS ) {
        _currentMenu = Menu(MENU_CLOCK_ID, _clockTitle, ARRAYSIZE(_clockItems), _clockItems);
        _menuChangedCallback();
    } else {
        WhiteClock.setValues(0, 0);
        BlackClock.setValues(0, 0);
        _currentMenu = Menu();
        _menuChangedCallback();
    }
}

void GameSettings::_setClock() {
    uint8_t selectedClock = _currentMenu.getSelectedIndex();
    switch (selectedClock) {
        case 0:
            WhiteClock.setValues(0, 0);
            BlackClock.setValues(0, 0);
            _currentMenu = Menu(MENU_RESTART_GAME_ID, _restartGameTitle, ARRAYSIZE(_restartGameItems), _restartGameItems);
            _paramsSettedCallback();
            break;
        case 1:
        case 2:
        case 3:
            WhiteClock.setValues(_clockItems[selectedClock]);
            BlackClock.setValues(_clockItems[selectedClock]);
            _finishConfigurations();
            break;
        case 4:
            _currentMenu = Menu();
            _menuChangedCallback();
            break;
        default:
            break;
    }
}

void GameSettings::_setCustomClock(bool isLongPress) {
    if( _clockSetter.isDone() ) {
        WhiteClock.setValues(_clockSetter.getWMinutes(), _clockSetter.getWIncrement());
        BlackClock.setValues(_clockSetter.getBMinutes(), _clockSetter.getBIncrement());
        _currentMenu = Menu(MENU_RESTART_GAME_ID, _restartGameTitle, ARRAYSIZE(_restartGameItems), _restartGameItems);
        _paramsSettedCallback();
    } else {
        _clockSetter.selectButton(isLongPress);
        _menuChangedCallback();
    }
}

void GameSettings::_finishConfigurations() {
    _currentMenu = Menu(MENU_RESTART_GAME_ID, _restartGameTitle, ARRAYSIZE(_restartGameItems), _restartGameItems);
    _paramsSettedCallback();
}

void GameSettings::restart() {
    _currentMenu = Menu(MENU_MODE_ID, _modeTitle, ARRAYSIZE(_modeItems), _modeItems);
    _clockSetter = ClockSetter();
    _gameMode = 0;
    _humanColor = 0;
    _aiELO = 0;
    WhiteClock.unset();
    BlackClock.unset();
}