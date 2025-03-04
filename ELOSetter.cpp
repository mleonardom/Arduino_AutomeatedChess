#include "ELOSetter.h"

ELOSetter::ELOSetter() {
}

void ELOSetter::prevButton(bool isLongPress) {
    _selectedElo = ( _selectedElo+_eloStep > _maxElo )? _minElo : _selectedElo+_eloStep;
}

void ELOSetter::nextButton(bool isLongPress) {
    _selectedElo = ( _selectedElo-_eloStep < _minElo )? _maxElo : _selectedElo-_eloStep;
}

uint16_t ELOSetter::getSelectedElo() {
    return _selectedElo;
}