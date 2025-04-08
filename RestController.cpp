#include "RestController.h"

RestController::RestController() {
}

String RestController::initGame() {
    String stockfish_move;

    _http.begin(_client, _AIServerName+"/chess/game");
    _http.addHeader("Content-Type", "application/json");

    JsonDocument body;
    body["user_color"] = _GameSettings.getHumanColorString();
    body["elo"] = _GameSettings.getELOSetter().getSelectedElo();

    String httpRequestData;
    serializeJson(body, httpRequestData);
    int httpResponseCode = _http.PUT(httpRequestData);
    if (httpResponseCode == 200) {
        DynamicJsonDocument resp = _readResponse(_http.getString());
        stockfish_move = resp["stockfish_move"].as<String>();
        if( stockfish_move=="null" ) stockfish_move = "";
    }

    _http.end();

    return stockfish_move;

}

String RestController::userMove(String move) {
    String stockfish_move;

    _http.begin(_client, _AIServerName+"/chess/game");
    _http.addHeader("Content-Type", "application/json");

    JsonDocument body;
    body["move"] = move;

    String httpRequestData;
    serializeJson(body, httpRequestData);
    int httpResponseCode = _http.POST(httpRequestData);
    if (httpResponseCode == 200) {
        DynamicJsonDocument resp = _readResponse(_http.getString());
        stockfish_move = resp["stockfish_move"].as<String>();
        if( stockfish_move=="null" ) stockfish_move = "";
    }

    _http.end();

    return stockfish_move;
}

DynamicJsonDocument RestController::_readResponse(String response) {
    Serial.println(response);
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonDocument doc(capacity);

    // Parse JSON object
    DeserializationError error = deserializeJson(doc, response);
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
    }

    return doc;
}