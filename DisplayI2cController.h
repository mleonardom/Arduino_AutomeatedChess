#ifndef DisplayI2cController_h
#define DisplayI2cController_h

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

#include "Menu.h"
#include "ClockSetter.h"
#include "ChessClock.h"
#include "Game.h"
#include "BoardController.h"
#include "Configs.h"
#include "BITMAPS.h"

class DisplayI2cController {
    private:
        LiquidCrystal_I2C lcd;
        byte _wifiChar[8] = {B10100,B11101,B11101,B00000,B11100,B10001,B11001,B10001};
        byte _skullChar[8] = {B00000,B01110,B10101,B11011,B01110,B01110,B00000,B00000};
        byte _downArrow[8] = {B00000,B00100,B00100,B00100,B00100,B10101,B01110,B00100};
        byte _upArrow[8] = {B00100,B01110,B10101,B00100,B00100,B00100,B00100,B00000};
        byte _checkChar[8] = {B00000,B00001,B00011,B10110,B11100,B01000,B00000,B00000};
        byte _blackPawn[8] = {B01110,B10001,B10001,B01010,B01010,B10001,B10001,B11111};
        byte _whitePawn[8] = {B01110,B11111,B11111,B01110,B01110,B11111,B11111,B11111};

        bool _isWiFiConnected = false;
        bool _showingClock = false;

        void _displayHumanVsHumanScreen();
        void _displayClocks();
    public:
        DisplayI2cController();
        void setup();
        void displayWelcomeScreen();
        void displayWebServerScreen();
        void displayMessage(String message);
        void displayMenu(Menu menu);
        void displayClockSetter(ClockSetter clockSetter);
        void displayELOSetter(ELOSetter eloSetter);
        void displayGame(Game game);
        void updateConnectionState(bool isConnected);
};

#endif