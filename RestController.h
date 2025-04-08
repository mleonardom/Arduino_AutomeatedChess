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
        String _AIServer;

        DynamicJsonDocument _readResponse(String response);
    public:
        RestController();
        void setAIServer(String AIServer);
        String initGame();
        String userMove(String move);
};

#endif