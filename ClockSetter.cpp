#include "ClockSetter.h"

ClockSetter::ClockSetter() {

}

String ClockSetter::getStringForNumber(uint8_t number) {
    return number < 10? "0"+((String)number) : (String)number;
}

uint8_t ClockSetter::getSetStep() {
    return _setStep;
}

void ClockSetter::prevButton(bool isLongPress) {
    uint8_t step = isLongPress? 10 : 1;
    switch(_setStep) {
        case 0:
            _wMinutes = _wMinutes+step>_maxMinutes? 0 : _wMinutes+step;
            _bMinutes = _bMinutes+step>_maxMinutes? 0 : _bMinutes+step;
            break;
        case 1:
            _wIncrement = _wIncrement+step>_maxIncrement? 0 : _wIncrement+step;
            _bIncrement = _bIncrement+step>_maxIncrement? 0 : _bIncrement+step;
            break;
        case 2:
            _wMinutes = _wMinutes+step>_maxMinutes? 0 : _wMinutes+step;
            break;
        case 3:
            _wIncrement = _wIncrement+step>_maxIncrement? 0 : _wIncrement+step;
            break;
        default:
            break;
    }
}

void ClockSetter::nextButton(bool isLongPress) {
    uint8_t step = isLongPress? 10 : 1;
    switch(_setStep) {
        case 0:
            _wMinutes = _wMinutes-step<0? _maxMinutes :_wMinutes-step;
            _bMinutes = _bMinutes-step<0? _maxMinutes :_bMinutes-step;
            break;
        case 1:
            _wIncrement = _wIncrement-step<0? _maxIncrement :_wIncrement-step;
            _bIncrement = _bIncrement-step<0? _maxIncrement :_bIncrement-step;
            break;
        case 2:
            _wMinutes = _wMinutes-step<0? _maxMinutes :_wMinutes-step;
            break;
        case 3:
            _wIncrement = _wIncrement-step<0? _maxIncrement :_wIncrement-step;
            break;
        default:
            break;
    }
}

void ClockSetter::selectButton(bool isLongPress) {
    _setStep ++;
}

String ClockSetter::getWMinutesString() {
    return getStringForNumber(_wMinutes);
}

String ClockSetter::getWIncrementString() {
    return getStringForNumber(_wIncrement);
}

String ClockSetter::getBMinutesString() {
    return getStringForNumber(_bMinutes);
}

String ClockSetter::getBIncrementString() {
    return getStringForNumber(_bIncrement);
}

uint8_t ClockSetter::getWMinutes() {
    return _wMinutes;
}

uint8_t ClockSetter::getWIncrement() {
    return _wIncrement;
}

uint8_t ClockSetter::getBMinutes() {
    return _bMinutes;
}

uint8_t ClockSetter::getBIncrement() {
    return _bIncrement;
}

bool ClockSetter::isDone() {
    return _setStep == 3;
}