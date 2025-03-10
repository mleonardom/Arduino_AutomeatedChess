#include "Configs.h"

#include "SoundController.h"
#include "WifiController.h"
#include "MultiButtonsController.h"
#include "BoardController.h"

#include "Game.h"
#include "GameSettings.h"

#ifdef USE_I2C
#include "DisplayI2cController.h"
#else
#include "DisplaySSD1306Controller.h"
#endif

// PIN Definitions
#define BUTTONS_PIN                         36
#define BUTTON1_PIN                         39
#define BUTTON2_PIN                         34

#define MOTOR_H_DIR                         5
#define MOTOR_H_STEP                        18
#define MOTOR_V_DIR                         15
#define MOTOR_V_STEP                        4

#define ELECTROMAGNET_PIN                   2

// PCB is maked for {13, 19, 14, 27}, make a manual wire on PCB to avoid Screen errors
uint8_t MUX_ADDR[4]                         = {23, 19, 14, 27};
uint8_t MUX_OUTPUTS[4]                      = {26, 25, 33, 32};

// Buttons definitions
#define UP_BUTTON                           0
#define DOWN_BUTTON                         2
#define SELECT_BUTTON                       1

// Buttons configs
int voltageRanges[][2] = {
    {2600,3000},
    {1600,2000},
    {600,1000}
};
uint8_t btnCount = ARRAYSIZE(voltageRanges);
unsigned long buttonCurTime;
unsigned long buttonLastLoop = 0;
unsigned int buttonLoopInterval = 400;

WiFiController _WiFiController;
SoundController _SoundController;
MultiButtonsController _MultiButtonsController(BUTTONS_PIN, btnCount, voltageRanges, 4095);
BoardController _BoardController(BUTTON1_PIN, BUTTON2_PIN, MUX_ADDR, MUX_OUTPUTS, MOTOR_H_DIR, MOTOR_H_STEP, MOTOR_V_DIR, MOTOR_V_STEP, ELECTROMAGNET_PIN);

#ifdef USE_I2C
DisplayI2cController _DisplayController;
#else
DisplaySSD1306Controller _DisplayController;
#endif

// Game
Game _Game;

void setup() {

    Serial.begin(115200);
    while (!Serial) { }

    pinMode(BUTTON1_PIN, INPUT);
    pinMode(BUTTON2_PIN, INPUT);

    _SoundController.setup();
    _BoardController.setup();

    _DisplayController.setup();
    _DisplayController.displayWelcomeScreen();
    delay(100);
    _SoundController.playStartSound();

    _MultiButtonsController.setup();
	_MultiButtonsController.setPressCallback(buttonPressCallback);
	_MultiButtonsController.setReleaseCallback(buttonReleaseCallback);

    delay(3000);

    _WiFiController.setup();
    _WiFiController.setWebServerCallback(wiFiWebServerCallback);
    bool resetWifi = _MultiButtonsController.isButtonPressed(1);
    bool wifiRes = _WiFiController.autoConnect(resetWifi);
	delay(300);
	if( wifiRes ) {
		Serial.println("WiFi Connected");
	} else {
        _DisplayController.displayMessage("WiFi ERROR!");
		Serial.println("WiFi ERROR!");
	}

    _GameSettings.setParamsSettedCallback(gameParamsSettedCallback);
    _GameSettings.setMenuChangedCallback(gameMenuChangedCallback);
    _GameSettings.setRestartGameCallback(gameRestartGameCallback);
    _Game.setGameChangedCallback(gameChangedCallback);
    _Game.setWhiteTimeoutCallback(whiteTimeoutCallback);
    _Game.setBlackTimeoutCallback(blackTimeoutCallback);

    _DisplayController.displayMenu(_GameSettings.getMenu());
}

void loop() {
    _WiFiController.loop();
    _MultiButtonsController.loop();
    _Game.loop();
    buttonsLoop();
    if( _WiFiController.hasChanges() ) {
        _DisplayController.updateConnectionState(_WiFiController.isConnected());
        if( _WiFiController.hasLostConnection() ) {
            _SoundController.playLowSignalSound();
        }
    }
}

void buttonsLoop() {
    if( digitalRead(BUTTON1_PIN) == HIGH ) {
        buttonCurTime = millis();
        if( buttonCurTime - buttonLastLoop >= buttonLoopInterval ) {
            buttonLastLoop = buttonCurTime;
            button1Pressed();
        }
    }
    if( digitalRead(BUTTON2_PIN) == HIGH ) {
        buttonCurTime = millis();
        if( buttonCurTime - buttonLastLoop >= buttonLoopInterval ) {
            buttonLastLoop = buttonCurTime;
            button2Pressed();
        }
    }
}

void button1Pressed() {
    _Game.startBlackClock();
}

void button2Pressed() {
    _Game.startWhiteClock();
}

void wiFiWebServerCallback () {
    Serial.println("Entered WebServer mode");
    _SoundController.playLowSignalSound();
    _DisplayController.displayWebServerScreen();
}

void buttonPressCallback(uint buttonId) {
    Serial.print("Button Press: ");
	Serial.println(buttonId);
}

void buttonReleaseCallback(uint buttonId, bool isLongPress) {
    Serial.print("Button released: ");
    Serial.println(buttonId);

    if( buttonId == UP_BUTTON ) {
        prevButton(isLongPress);
    } else if (buttonId == DOWN_BUTTON) {
        nextButton(isLongPress);
    } else if (buttonId == SELECT_BUTTON) {
        selectButton(isLongPress);
    }
    
}

void gameParamsSettedCallback() {
    startGame();
}

void gameMenuChangedCallback() {
    if( _GameSettings.getMenu().getID() == 0 ) {
        if( _GameSettings.isClocksSetted() ) {
            _DisplayController.displayELOSetter(_GameSettings.getELOSetter());
        } else {
            _DisplayController.displayClockSetter(_GameSettings.getClockSetter());
        }
    } else {
        _DisplayController.displayMenu(_GameSettings.getMenu());
    }
}

void prevButton(bool isLongPress) {
    if( !_Game.isInGame() && !_Game.isGameFinished() ) {
        _SoundController.playPieceSound();
        _GameSettings.prevButton(isLongPress);
    } else {
        Serial.println("Disabled Prev button");
    }
}

void nextButton(bool isLongPress) {
    if( !_Game.isInGame() && !_Game.isGameFinished() ) {
        _SoundController.playPieceSound();
        _GameSettings.nextButton(isLongPress);
    } else {
        Serial.println("Disabled Next button");
    }
}

void selectButton(bool isLongPress) {
    if( !_Game.isInGame() ) {
        _SoundController.playSelectUISound();
        if( _Game.isGameFinished() ) {
            restartGame();
        } else {
            _GameSettings.selectButton(isLongPress);
        }
    } else if(isLongPress) {
        pauseGame();
        _SoundController.playLowSignalSound();
        _DisplayController.displayMenu(_GameSettings.getMenu());
    } else {
        Serial.println("Unactive Select button");
    }
}

void gameRestartGameCallback() {
    restartGame();
}

void startGame() {
    if( _GameSettings.getGameMode() == G_MODE_H_C ) {
        _DisplayController.displayMessage("Calibrando ...");
        _BoardController.calibrate();
    }
    _Game.start();
}

void restartGame() {
    _GameSettings.restart();
    _Game.restart();
    _DisplayController.displayMessage("Reiniciando Juego...");
    delay(1000);
    _DisplayController.displayMenu(_GameSettings.getMenu());
}

void pauseGame() {
    _Game.pause();
}

void gameChangedCallback() {
    _DisplayController.displayGame(_Game);
}

void whiteTimeoutCallback() {
    _SoundController.playTimeoutSound();
    _Game.gameOver();
}

void blackTimeoutCallback() {
    _SoundController.playTimeoutSound();
    _Game.gameOver();
}