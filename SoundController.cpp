#include "SoundController.h"

SoundController::SoundController() {
    DFPlayerMini_Fast _DFPlayer;
}

void SoundController::setup() {
    FPSerial.begin(9600, SERIAL_8N1, 16, 17);
    while (!_DFPlayer.begin(FPSerial)) {
        Serial.print(".");
        delay(1000);
    }
    Serial.print("MP3 files in SD: ");
    Serial.println(_DFPlayer.numSdTracks());

    Serial.print("MP3 files in Flash: ");
    Serial.println(_DFPlayer.numFlashTracks());
    // Set the DFPlayer Mini volume to 30 (max is 30)
    _DFPlayer.volume(30);
}

void SoundController::play(uint16_t trackNum) {
    Serial.print("Playing track No.: ");
    Serial.println(trackNum);
    _DFPlayer.play(trackNum);
}

void SoundController::playStartSound() {
    play(START_SOUND_INDEX);
}

void SoundController::playErrorSound() {
    play(ERROR_SOUND_INDEX);
}

void SoundController::playCheckmateSound() {
    play(CHECKMATE_SOUND_INDEX);
}

void SoundController::playPieceSound() {
    play(PIECE_SOUND_INDEX);
}

void SoundController::playNotification1Sound() {
    play(NOTIFICVATION1_SOUND_INDEX);
}

void SoundController::playLowSignalSound() {
    play(LOW_SIGNAL_SOUND_INDEX);
}

void SoundController::playTimeoutSound() {
    play(TIMEOUT_SOUND_INDEX);
}