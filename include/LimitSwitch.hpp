#include <Arduino.h>
#pragma once

using LimitCallback = void(*)();

class LimitSwitch {
public:
    LimitSwitch(uint8_t pin, LimitCallback onPress);

    void update();

private:
    uint8_t pin;
    bool lastState;
    bool triggered;
    unsigned long lastDebounceTime;
    LimitCallback onPress;

    static const unsigned long debounceDelay = 50;
};