#ifndef DisplaySSD1306Controller_h
#define DisplaySSD1306Controller_h

#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Menu.h"
#include "ClockSetter.h"
#include "ChessClock.h"
#include "Game.h"
#include "Configs.h"

#include "BITMAPS.h"
#include "BoardController.h"

#define SCREEN_WIDTH                128
#define SCREEN_HEIGHT               64
#define OLED_RESET                  -1

#define TITLE_X                     1
#define TITLE_Y                     1
#define TITLE_HEIGHT                10

#define CLOCK_SETTER_MARGIN_TOP     15

class DisplaySSD1306Controller {
    private:
        Adafruit_SSD1306 _display;
        bool _isWiFiConnected = false;
        void _drawCentreString(const String &buf, uint16_t textSize);
        void _drawCentreStringTwoLines(const String &buf1, const String &buf2);
        void _drawCentreBitmap(const uint8_t *bitmap, uint8_t w, uint8_t h);
        void _displayClocks();
        void _drawHeader(String title);
        void _displayHumanVsHumanScreen();
        void _displayHumanVsAIScreen(Game &game);

    public:
        DisplaySSD1306Controller();
        void setup();
        void displayWelcomeScreen();
        void displayWebServerScreen();
        void displayMessage(String message);
        void displayMenu(Menu menu);
        void displayClockSetter(ClockSetter clockSetter);
        void displayELOSetter(ELOSetter eloSetter);
        void displayGame(Game &game);
        void updateConnectionState(bool isConnected);
};

#endif