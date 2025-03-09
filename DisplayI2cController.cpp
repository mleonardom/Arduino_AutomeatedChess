#include "DisplayI2cController.h"

DisplayI2cController::DisplayI2cController() : lcd(0x27,20,4) {
}

void DisplayI2cController::setup(){
    lcd.init();
    lcd.backlight();
    lcd.createChar(0, _skullChar);
    lcd.createChar(1, _wifiChar);
    lcd.createChar(2, _downArrow);
    lcd.createChar(3, _upArrow);
    lcd.createChar(4, _checkChar);
    lcd.createChar(5, _whitePawn);
    lcd.createChar(6, _blackPawn);
}

void DisplayI2cController::displayWelcomeScreen(){
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print(">> AUTO CHESS <<");
    lcd.setCursor(1, 2);
    lcd.print("By Leonardo Mendez");
    _showingClock = false;
}

void DisplayI2cController::displayWebServerScreen(){
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Configurar WiFi");
    lcd.setCursor(0, 2);
    lcd.print("Red: ");
    lcd.setCursor(5, 2);
    lcd.print(MD_DEVICE_NAME);
    _showingClock = false;
}

void DisplayI2cController::displayMessage(String message){
    lcd.clear();
    lcd.setCursor(0, 2);
    lcd.print(message);
    updateConnectionState(_isWiFiConnected);
    _showingClock = false;
}

void DisplayI2cController::displayMenu(Menu menu){
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print(menu.getTitle());

    uint8_t initial_item = 0;
    uint8_t left_margin = 1;
    uint8_t top_margin = 1;
    uint8_t showed_items = menu.getItemsCount() > 3? 3 : menu.getItemsCount();
    if( menu.getSelectedIndex() >= 3 ) {
        lcd.setCursor(19, 1);
        lcd.write(3);
        initial_item = menu.getSelectedIndex() - 2;
    }
    if( menu.getItemsCount() > 3 ){
        lcd.setCursor(19, 3);
        lcd.write(2);
    } else if( menu.getItemsCount() < 3 ) {
        top_margin = 2;
    }
    for( uint8_t i=0; i<showed_items; i++ ) {
        left_margin = menu.getSelectedIndex() == i+initial_item? 1 : 2;
        lcd.setCursor(left_margin, i+top_margin);
        lcd.print(menu.getLabelItem(i+initial_item));
    }
    updateConnectionState(_isWiFiConnected);
    _showingClock = false;
}

void DisplayI2cController::displayClockSetter(ClockSetter clockSetter){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Configurar Reloj");

    lcd.setCursor(1, 2);
    lcd.write(6);
    lcd.setCursor(2, 2);
    lcd.print(":");

    lcd.setCursor(12, 2);
    lcd.write(5);
    lcd.setCursor(13, 2);
    lcd.print(":");

    lcd.setCursor(3, 2);
    lcd.print(clockSetter.getBMinutesString());
    lcd.setCursor(5, 2);
    lcd.print("+");
    lcd.setCursor(6, 2);
    lcd.print(clockSetter.getBIncrementString());

    lcd.setCursor(14, 2);
    lcd.print(clockSetter.getWMinutesString());
    lcd.setCursor(16, 2);
    lcd.print("+");
    lcd.setCursor(17, 2);
    lcd.print(clockSetter.getWIncrementString());

    uint8_t indicator_x = 0;
    switch( clockSetter.getSetStep() ) {
        case 0:
            indicator_x = 3;
            break;
        case 1:
            indicator_x = 6;
            break;
        case 2:
            indicator_x = 14;
            break;
        case 3:
            indicator_x = 17;
            break;
    }
    lcd.setCursor(indicator_x, 3);
    lcd.print("^^");

    updateConnectionState(_isWiFiConnected);
    _showingClock = false;
}

void DisplayI2cController::displayELOSetter(ELOSetter eloSetter){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ELO de la IA");

    lcd.setCursor(8, 3);
    lcd.print("^^^^");

    lcd.setCursor(8, 2);
    lcd.print(eloSetter.getSelectedElo());

    updateConnectionState(_isWiFiConnected);
    _showingClock = false;
}

void DisplayI2cController::displayGame(Game game){
    if( game.getGameState() == GAME_WAITING_CLOCK_STATUS ) {
        displayMessage("Inicie reloj blancas");
    } else if( game.hasClocks() ){
        _displayClocks();
    } else {
        _displayHumanVsHumanScreen();
    }
}

void DisplayI2cController::updateConnectionState(bool isConnected){
    int8_t iconIndex = isConnected? 4 : 0;
    lcd.setCursor(18, 0);
    lcd.write(1);
    lcd.setCursor(19, 0);
    lcd.write(iconIndex);
    _isWiFiConnected = isConnected;
}

void DisplayI2cController::_displayHumanVsHumanScreen() {
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("Humano Vs Humano");
    lcd.setCursor(5, 2);
    lcd.print("Sin Reloj");

    updateConnectionState(_isWiFiConnected);
    _showingClock = false;
}

void DisplayI2cController::_displayClocks() {
    if( !_showingClock ) {
        lcd.clear();

        lcd.setCursor(0, 1);
        lcd.print("----------|---------");
        lcd.setCursor(10, 2);
        lcd.print("|");
        lcd.setCursor(10, 3);
        lcd.print("|");

        lcd.setCursor(0, 3);
        lcd.write(6);

        lcd.setCursor(11, 3);
        lcd.write(5);

        lcd.setCursor(0, 0);
        lcd.print(BlackClock.getInitialMinutesString());
        lcd.setCursor(2, 0);
        lcd.print("+");
        lcd.setCursor(3, 0);
        lcd.print(BlackClock.getIncrementString());
        lcd.setCursor(5, 0);
        lcd.print("-");
        lcd.setCursor(6, 0);
        lcd.print(WhiteClock.getInitialMinutesString());
        lcd.setCursor(8, 0);
        lcd.print("+");
        lcd.setCursor(9, 0);
        lcd.print(WhiteClock.getIncrementString());

        updateConnectionState(_isWiFiConnected);
    }

    lcd.setCursor(1, 2);
    lcd.print(BlackClock.getRemainingMinutesString());
    lcd.setCursor(3, 2);
    lcd.print(":");
    lcd.setCursor(4, 2);
    lcd.print(BlackClock.getRemainingSecondsString());
    lcd.setCursor(14, 2);
    lcd.print(WhiteClock.getRemainingMinutesString());
    lcd.setCursor(16, 2);
    lcd.print(":");
    lcd.setCursor(17, 2);
    lcd.print(WhiteClock.getRemainingSecondsString());

    lcd.setCursor(1, 3);
    lcd.print(BlackClock.getMoves());
    lcd.setCursor(12, 3);
    lcd.print(WhiteClock.getMoves());
    
    _showingClock = true;
}
