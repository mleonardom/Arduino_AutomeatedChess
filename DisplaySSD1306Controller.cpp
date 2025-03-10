#include "DisplaySSD1306Controller.h"

DisplaySSD1306Controller::DisplaySSD1306Controller() :
    _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) {
}

void DisplaySSD1306Controller::setup() {
    if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for(;;);
    }
    delay(500);
    _display.clearDisplay();
    _display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    _display.display();
}

void DisplaySSD1306Controller::displayWelcomeScreen() {
    _display.clearDisplay();
    _drawCentreBitmap(epd_bitmap_Splash, 58, 62);
    _display.display();
}

void DisplaySSD1306Controller::displayWebServerScreen() {
    _display.clearDisplay();
    _drawCentreStringTwoLines("Configurar WiFi:", "\""+String(MD_DEVICE_NAME)+"\"");
    _display.display();
}

void DisplaySSD1306Controller::displayMessage(String message) {
    _display.clearDisplay();
    _drawCentreString(message, 1);
    _display.display();
}

void DisplaySSD1306Controller::displayMenu(Menu menu) {
    _display.clearDisplay();

    _drawHeader(menu.getTitle());

    _display.setTextColor(WHITE);
    _display.setTextSize(1);

    int16_t x1, y1;
    uint16_t width, height;
    _display.getTextBounds(menu.getTitle(), 0, 0, &x1, &y1, &width, &height);
    uint8_t itemSpace = 1;
    uint8_t itemPadding = 4;
    uint8_t itemX, itemY, menuX, menuY;
    menuX = itemSpace;
    uint8_t itemHeight = (2*itemSpace)+height-1;
    menuY = ((SCREEN_HEIGHT - TITLE_HEIGHT - (menu.getItemsCount()*itemHeight)) /2) + TITLE_HEIGHT + itemSpace;
    for( uint8_t i=0; i<menu.getItemsCount(); i++ ) {
        itemY = menuY + (itemHeight*i);
        if( menu.getSelectedIndex() == i ) {
            _display.fillRect(itemX, itemY, SCREEN_WIDTH-(2*menuX), itemHeight, WHITE);
            _display.setTextColor(BLACK);
        } else {
            _display.setTextColor(WHITE);
        }
        _display.setCursor( itemX+itemPadding, itemY+1 );
        _display.println(menu.getLabelItem(i));
    }

    updateConnectionState(_isWiFiConnected);
}

void DisplaySSD1306Controller::_drawCentreBitmap(const uint8_t *bitmap, uint8_t width, uint8_t height) {
    _display.drawBitmap((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2, bitmap, width, height, WHITE);
}

void DisplaySSD1306Controller::_drawCentreString(const String &buf, uint16_t textSize) {
    int16_t x1, y1;
    uint16_t width, height;

    _display.setTextColor(WHITE);
    _display.setTextSize(textSize);
    _display.getTextBounds(buf, 0, 0, &x1, &y1, &width, &height);
    _display.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
    _display.println(buf);
}

void DisplaySSD1306Controller::_drawCentreStringTwoLines(const String &buf1, const String &buf2) {
    int16_t x1, y1;
    uint16_t width, height;
    uint16_t margin = 2;

    _display.setTextColor(WHITE);
    _display.setTextSize(1);
    _display.getTextBounds(buf1, 0, 0, &x1, &y1, &width, &height);
    _display.setCursor((SCREEN_WIDTH - width) / 2, ((SCREEN_HEIGHT - margin) / 2)-height);
    _display.println(buf1);
    _display.getTextBounds(buf2, 0, 0, &x1, &y1, &width, &height);
    _display.setCursor((SCREEN_WIDTH - width) / 2, ((SCREEN_HEIGHT + margin) / 2));
    _display.println(buf2);
}
void DisplaySSD1306Controller::displayClockSetter(ClockSetter clockSetter) {
    _display.clearDisplay();
    _drawHeader("Configurar Relojes");

    int16_t x1, y1;
    uint16_t numbersWidth, numbersHeight, plusWidth, plusHeight;
    _display.setTextColor(WHITE);
    _display.setTextSize(2);
    _display.getTextBounds("00", 0, 0, &x1, &y1, &numbersWidth, &numbersHeight);
    _display.getTextBounds("+", 0, 0, &x1, &y1, &plusWidth, &plusHeight);
    uint8_t bitmapMargin = 1;
    uint8_t textY = CLOCK_SETTER_MARGIN_TOP+8+bitmapMargin;
    _display.setCursor(0, textY);
    _display.print(clockSetter.getBMinutesString());
    _display.print("+");
    _display.println(clockSetter.getBIncrementString());

    _display.setCursor(SCREEN_WIDTH-(numbersWidth*2)-plusWidth, textY);
    _display.print(clockSetter.getWMinutesString());
    _display.print("+");
    _display.println(clockSetter.getWIncrementString());

    uint8_t bitmapX = 0;
    switch( clockSetter.getSetStep() ) {
        case 0:
            bitmapX = 1;
            break;
        case 1:
            bitmapX = numbersWidth+plusWidth+1;
            break;
        case 2:
            bitmapX = SCREEN_WIDTH-(numbersWidth*2)-plusWidth+1;
            break;
        case 3:
            bitmapX = SCREEN_WIDTH-23;
            break;
    }
    _display.drawBitmap(bitmapX, CLOCK_SETTER_MARGIN_TOP, epd_bitmap_NumberUpValue, 20, 8, WHITE);
    _display.drawBitmap(bitmapX, textY+numbersHeight+bitmapMargin-2, epd_bitmap_NumberDownValue, 20, 8, WHITE);
    
    _display.drawFastVLine(SCREEN_WIDTH/2, TITLE_HEIGHT, SCREEN_HEIGHT-TITLE_HEIGHT, WHITE);
    
    _display.drawBitmap(3, 50, epd_bitmap_BlackPawn, 9, 12, WHITE);
    _display.drawBitmap(68, 50, epd_bitmap_WhitePawn, 9, 12, WHITE);
    
    _display.setTextSize(1);
    _display.setCursor(15, 55);
    _display.print("Negras");
    _display.setCursor(80, 55);
    _display.print("Blancas");

    updateConnectionState(_isWiFiConnected);
}

void DisplaySSD1306Controller::displayGame(Game &game) {
    if( game.getGameState() == GAME_WAITING_CLOCK_STATUS ) {
        displayMessage("Inicie reloj blancas");
    } else if( game.hasClocks() ){
        _displayClocks();
    } else if( _GameSettings.getGameMode() == G_MODE_H_H ){
        _displayHumanVsHumanScreen();
    } else if( _GameSettings.getGameMode() == G_MODE_H_C ){
        _displayHumanVsAIScreen(game);
    }
}

void DisplaySSD1306Controller::_drawHeader(String title) {
    _display.drawFastHLine(0, TITLE_HEIGHT, SCREEN_WIDTH, WHITE);
    _display.setTextColor(WHITE);
    _display.setTextSize(1);
    _display.setCursor(TITLE_X, TITLE_Y);
    _display.println(title);
}

void DisplaySSD1306Controller::_displayClocks() {
    _display.clearDisplay();

    _drawHeader("Reloj");

    _display.setTextColor(WHITE);
    _display.setTextSize(1);
    int16_t x1, y1;
    uint16_t width, height;
    _display.getTextBounds("00+00", 0, 0, &x1, &y1, &width, &height);
    _display.setCursor(0, 16);
    _display.print(BlackClock.getInitialMinutesString());
    _display.print("+");
    _display.print(BlackClock.getIncrementString());

    _display.setCursor(SCREEN_WIDTH-width, 16);
    _display.print(WhiteClock.getInitialMinutesString());
    _display.print("+");
    _display.print(WhiteClock.getIncrementString());

    _display.setTextSize(2);
    _display.getTextBounds("00+00", 0, 0, &x1, &y1, &width, &height);
    _display.setCursor(0, 31);
    _display.print(BlackClock.getRemainingMinutesString());
    _display.print(":");
    _display.print(BlackClock.getRemainingSecondsString());

    _display.setCursor(SCREEN_WIDTH-width, 31);
    _display.print(WhiteClock.getRemainingMinutesString());
    _display.print(":");
    _display.print(WhiteClock.getRemainingSecondsString());

    _display.drawFastVLine(SCREEN_WIDTH/2, TITLE_HEIGHT, SCREEN_HEIGHT-TITLE_HEIGHT, WHITE);

    _display.drawBitmap(3, 51, epd_bitmap_BlackPawn, 9, 12, WHITE);
    _display.drawBitmap(68, 51, epd_bitmap_WhitePawn, 9, 12, WHITE);

    _display.setTextSize(1);
    _display.setCursor(14, 56);
    _display.print(BlackClock.getMoves());
    _display.setCursor(79, 56);
    _display.print(WhiteClock.getMoves());

    updateConnectionState(_isWiFiConnected);
}

void DisplaySSD1306Controller::displayELOSetter(ELOSetter eloSetter) {
    _display.clearDisplay();
    _drawHeader("ELO de la AI");

    _display.setTextColor(WHITE);
    _display.setTextSize(2);

    int16_t x1, y1;
    uint16_t numbersWidth, numbersHeight, numbersX, numbersY;
    _display.getTextBounds("0000", 0, 0, &x1, &y1, &numbersWidth, &numbersHeight);
    numbersX = (SCREEN_WIDTH - numbersWidth) / 2;
    numbersY = 30;
    _display.setCursor(numbersX, numbersY);
    _display.println(eloSetter.getSelectedElo());

    uint8_t bitmapMargin = 2;
    _display.drawBitmap(numbersX+2, numbersY-bitmapMargin-8, epd_bitmap_NumberUpValueBig, 40, 8, WHITE);
    _display.drawBitmap(numbersX+2, numbersY+numbersHeight+bitmapMargin-2, epd_bitmap_NumberDownValueBig, 40, 8, WHITE);

    updateConnectionState(_isWiFiConnected);
}

void DisplaySSD1306Controller::_displayHumanVsHumanScreen() {
    _display.clearDisplay();
    _drawHeader("En Juego");
    _display.setTextColor(WHITE);
    _display.setTextSize(2);
    int16_t x1, y1;
    uint16_t width, height;
    _display.getTextBounds("Vs", 0, 0, &x1, &y1, &width, &height);
    _display.setCursor((SCREEN_WIDTH - width) / 2, 37);
    _display.println("Vs");

    _display.drawBitmap(12, 25, epd_bitmap_Human, 36, 39, WHITE);
    _display.drawBitmap(80, 25, epd_bitmap_Human, 36, 39, WHITE);

    updateConnectionState(_isWiFiConnected);
}

void DisplaySSD1306Controller::_displayHumanVsAIScreen(Game &game) {

    _display.clearDisplay();
    _drawHeader("En Juego");

    _drawCentreString("En Juego", 1);

    updateConnectionState(_isWiFiConnected);
}

void DisplaySSD1306Controller::updateConnectionState(bool isConnected) {
    _display.fillRect(116, 1, 11, 7, BLACK);
    if( isConnected ) {
        _display.drawBitmap(116, 1, epd_bitmap_WiFi, 11, 7, WHITE);
    } else {
        _display.drawBitmap(116, 1, epd_bitmap_WiFierror, 11, 7, WHITE);
    }

    _display.display();
    _isWiFiConnected = isConnected;
}
