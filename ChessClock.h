#ifndef ChessClock_h
#define ChessClock_h

#include "Arduino.h"
#include "CountDown.h"

class ChessClock {
    private:
        uint32_t _remainingTime;
        uint8_t _initialMinutes;
        uint8_t _increment;
        bool _isSetted = false;
        bool isFirstTurn = true;
        uint16_t _moves = 0;
        std::function<void()> _timeoutCallback;

        CountDown _CD;

        String getStringForNumber(uint8_t number);
    public:
        ChessClock();
        void setValues(const String str);
        void setValues(uint8_t minutes, uint8_t increment);
        bool loop();
        void setTimeoutCallback(std::function<void()> timeoutCallback);
        void unset();
        void start();
        void pause();
        void stop();
        bool isSetted();
        String getRemainingMinutesString();
        String getRemainingSecondsString();
        uint8_t getIncrement();
        uint8_t getInitialMinutes();
        String getIncrementString();
        String getInitialMinutesString();
        uint16_t getMoves();
        bool isRunning();
};

extern ChessClock WhiteClock;
extern ChessClock BlackClock;

#endif