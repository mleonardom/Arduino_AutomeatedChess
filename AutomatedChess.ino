#include "Configs.h"
#include <Preferences.h>

#include "SoundController.h"
#include "WifiController.h"
#include "MultiButtonsController.h"
#include "BoardController.h"

#include "Game.h"
#include "GameSettings.h"

#include "DisplaySSD1306Controller.h"

// PIN Definitions
#define BUTTONS_PIN                         36 //VP
#define BUTTON1_PIN                         39 //VN
#define BUTTON2_PIN                         2

#define MOTOR_H_DIR                         5
#define MOTOR_H_STEP                        18
#define MOTOR_V_DIR                         15
#define MOTOR_V_STEP                        4

#define ELECTROMAGNET_PIN_1                 25
#define ELECTROMAGNET_PIN_2                 26

uint8_t MUX_ADDR[4]                         = {23, 19, 14, 27};

uint8_t MUX_OUTPUTS[4]                      = {32, 33, 35, 34};

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

Preferences preferences;
WiFiController _WiFiController;
SoundController _SoundController;
MultiButtonsController _MultiButtonsController(BUTTONS_PIN, btnCount, voltageRanges, 4095);
BoardController _BoardController(BUTTON1_PIN, BUTTON2_PIN, MUX_ADDR, MUX_OUTPUTS, MOTOR_H_DIR, MOTOR_H_STEP, MOTOR_V_DIR, MOTOR_V_STEP, ELECTROMAGNET_PIN_1, ELECTROMAGNET_PIN_2);

DisplaySSD1306Controller _DisplayController;

// Game
Game _Game;

String AIServer = "";

void setup() {

    Serial.begin(115200);
    while (!Serial) { }

    pinMode(BUTTON1_PIN, INPUT);
    pinMode(BUTTON2_PIN, INPUT);

    preferences.begin(MD_DEVICE_NAME);
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
    _WiFiController.setSaveConfigCallback(wiFiSaveConfigCallback);
    bool resetWifi = digitalRead(BUTTON1_PIN) == HIGH && digitalRead(BUTTON2_PIN) == HIGH;
    bool wifiRes = _WiFiController.autoConnect(resetWifi);
	delay(300);
	if( wifiRes ) {
		Serial.println("WiFi Connected");
	} else {
        _DisplayController.displayMessage("WiFi ERROR!");
		Serial.println("WiFi ERROR!");
	}
    
    if( AIServer == "" ) {
        AIServer = preferences.getString("ai_server", "http://192.168.1.100:8081");
    }
    _Game.setAIServer(AIServer);
    _GameSettings.setParamsSettedCallback(gameParamsSettedCallback);
    _GameSettings.setMenuChangedCallback(gameMenuChangedCallback);
    _GameSettings.setRestartGameCallback(gameRestartGameCallback);
    _Game.setGameChangedCallback(gameChangedCallback);
    _Game.setWhiteTimeoutCallback(whiteTimeoutCallback);
    _Game.setBlackTimeoutCallback(blackTimeoutCallback);
    _Game.setAIMoveCallback(AImoveCallback);


    _DisplayController.displayMenu(_GameSettings.getMenu());
}

void loop() {
    _WiFiController.loop();
    _MultiButtonsController.loop();
    _Game.loop();
    _BoardController.loop();
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

void makeUserMovement() {
    String userMove = _BoardController.calculateUserMovement();
    if( _Game.putUserMovement(userMove) ) {
        Serial.print("User moves to:");
        Serial.println(userMove);
    } else {
        _SoundController.playErrorSound();
        Serial.println("User move invalid !!!!!");
    }
}

void button1Pressed() {
    Serial.println("Button Black Clock (1) pressed");
    if( !_Game.hasClocks() ) {
        if( _Game.getGameState() != GAME_WAITING_USER_MOVE || _GameSettings.getHumanColor() != G_HUMAN_COLOR_BLACK ) {
            Serial.println("Button Black Clock Disabled");
            return;
        }
        makeUserMovement();
    } else {
        _Game.startBlackClock();
    }
}

void button2Pressed() {
    Serial.println("Button White Clock (2) pressed");
    if( !_Game.hasClocks() ) {
        if( _Game.getGameState() != GAME_WAITING_USER_MOVE || _GameSettings.getHumanColor() != G_HUMAN_COLOR_WHITE ) {
            Serial.println("Button White Clock Disabled");
            return;
        }
        makeUserMovement();
    } else {
        _Game.startWhiteClock();
    }
}

void wiFiWebServerCallback () {
    Serial.println("Entered WebServer mode");
    _SoundController.playLowSignalSound();
    _DisplayController.displayWebServerScreen();
}

void wiFiSaveConfigCallback() {
    Serial.print("Save AIServer with:");
    Serial.println(_WiFiController.getServerParam());
    AIServer = _WiFiController.getServerParam();
    preferences.putString("ai_server", AIServer);
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
        //_SoundController.playPieceSound();
        _GameSettings.prevButton(isLongPress);
    } else {
        Serial.println("Disabled Prev button");
    }
}

void nextButton(bool isLongPress) {
    if( !_Game.isInGame() && !_Game.isGameFinished() ) {
        //_SoundController.playPieceSound();
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
        //_BoardController.calibrate();
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

void AImoveCallback(String move) {
    Serial.print("AI moves to: ");
    Serial.println(move);
    _BoardController.moveToString(move);
}