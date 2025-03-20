#ifndef RestController_h
#define RestController_h

#include "Arduino.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "ArduinoJson.h"

#include "Configs.h"
#include "GameSettings.h"

class RestController {
    private:
        NetworkClient _client;
        HTTPClient _http;

        // TODO make this configurable
        String _AIServerName = "http://192.168.86.100:8081";

        DynamicJsonDocument _readResponse(String response);
    public:
        RestController();
        String initGame();
};

#endif