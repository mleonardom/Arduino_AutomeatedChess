#ifndef WiFiController_h
#define WiFiController_h

#include <WiFiManager.h>
#include <HTTPClient.h>

#include "Configs.h"

class WiFiController {

	private:
		WiFiManager _wifiManager;
		String _lastWLStatus = "";

	public:
		WiFiController();
		void setup();
        void setWebServerCallback(std::function<void()> webServerCallback);
		bool autoConnect(bool reset);
        void setResponseCallback(std::function<void()> func);
		void loop();
		bool hasChanges();
		String getWLStatus();
		bool isConnected();
		bool hasLostConnection();
};

#endif