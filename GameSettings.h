#ifndef GameSettings_h
#define GameSettings_h

#include "Arduino.h"
#include "Configs.h"
#include "Menu.h"
#include "ClockSetter.h"
#include "ChessClock.h"
#include "ELOSetter.h"

enum {MENU_NO_MENU_ID, MENU_MODE_ID, MENU_HUMAN_COLOR_ID, MENU_MACHINE_ELO_ID, MENU_CLOCK_ID, MENU_RESTART_GAME_ID};

enum {G_MODE_H_H, G_MODE_H_C};

enum {G_HUMAN_COLOR_WHITE, G_HUMAN_COLOR_BLACK};

class GameSettings {

    private:
        uint8_t _gameMode = 0;
        uint8_t _humanColor = 0;
        uint16_t _aiELO = 0;

        const String _modeTitle = "Modo";
        const String _humanColorTitle = "Elige color";
        const String _clockTitle = "Relojes";
        const String _restartGameTitle = "Reiniciar Juego?";

        const String _modeItems[2] = {"Humano Vs Humano", "Humano Vs AI"};
        const String _humanColorItems[2] = {"Blancas", "Negras"};
        const String _clockItems[5] = {"Sin reloj", "10+0", "5+3", "5+0", "Personalizado"};
        const String _restartGameItems[2] = {"Cancelar", "Reiniciar"};

        std::function<void()> _paramsSettedCallback;
        std::function<void()> _menuChangedCallback;
        std::function<void()> _restartGameCallback;
        Menu _currentMenu;
        ClockSetter _clockSetter;
        ELOSetter _ELOSetter;

        void _restartSettings();
        void _setMode();
        void _setHumanColor();
        void _setAIELO();
        void _setClock();
        void _setCustomClock(bool isLongPress);
        void _finishConfigurations();
    public:
        GameSettings();
        bool hasSettings();
        bool isClocksSetted();
        Menu getMenu();
        ClockSetter getClockSetter();
        ELOSetter getELOSetter();
        void setParamsSettedCallback(std::function<void()> paramsSettedCallback);
        void setMenuChangedCallback(std::function<void()> menuChangedCallback);
        void setRestartGameCallback(std::function<void()> restartGameCallback);
        void prevButton(bool isLongPress);
        void nextButton(bool isLongPress);
        void selectButton(bool isLongPress);
        uint8_t getGameMode();
        uint16_t getAIELO();
        uint8_t getHumanColor();
        void restart();

};

extern GameSettings _GameSettings;

#endif