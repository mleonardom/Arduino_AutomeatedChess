#include "WifiController.h"

WiFiController::WiFiController() : _stockfishServerParameter("server", "RPI Server", "http://192.168.1.100:8081", 40) {
    WiFiManager _wifiManager;
}

void WiFiController::setup() {
    _wifiManager.setDebugOutput(true);
    std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
    _wifiManager.setMenu(menu);
    _wifiManager.setConfigPortalTimeout(CONFIGURE_PORTAL_TIMEOUT);
    Serial.println("Add WiFi Parameter");
    _wifiManager.addParameter(&_stockfishServerParameter);
    _wifiManager.setConnectTimeout(30);
}

void WiFiController::setWebServerCallback(std::function<void()> webServerCallback) {
    _wifiManager.setWebServerCallback(webServerCallback);
}

void WiFiController::setSaveConfigCallback(std::function<void()> saveConfigCallback) {
    _wifiManager.setSaveConfigCallback(saveConfigCallback);
}

bool WiFiController::autoConnect(bool reset) {
    if( reset ){
        _wifiManager.resetSettings();
    }

    bool res = _wifiManager.autoConnect(MD_DEVICE_NAME);

    return res;
}

void WiFiController::setResponseCallback(std::function<void()> func) {

}

void WiFiController::loop() {
	_wifiManager.process();
}

bool WiFiController::hasChanges() {
    if( _wifiManager.getWLStatusString() != _lastWLStatus ) {
        Serial.print("Updating WiFi Status to: ");
        Serial.println(_wifiManager.getWLStatusString());
        _lastWLStatus = _wifiManager.getWLStatusString();
        return true;
    }

    return false;
}

bool WiFiController::isConnected() {
    return _wifiManager.getWLStatusString() == "WL_CONNECTED";
}

bool WiFiController::hasLostConnection() {
    return _wifiManager.getWLStatusString() == "WL_DISCONNECTED";
}

String WiFiController::getServerParam() {
    return _stockfishServerParameter.getValue();
}