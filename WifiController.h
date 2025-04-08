#ifndef WiFiController_h
#define WiFiController_h

#include <WiFiManager.h>

#include "Configs.h"

class WiFiController {

	private:
		WiFiManager _wifiManager;
		String _lastWLStatus = "";
		WiFiManagerParameter _stockfishServerParameter;

	public:
		WiFiController();
		void setup();
        void setWebServerCallback(std::function<void()> webServerCallback);
		void setSaveConfigCallback(std::function<void()> saveConfigCallback);
		bool autoConnect(bool reset);
        void setResponseCallback(std::function<void()> func);
		void loop();
		bool hasChanges();
		bool isConnected();
		bool hasLostConnection();
		String getServerParam();
};

#endif