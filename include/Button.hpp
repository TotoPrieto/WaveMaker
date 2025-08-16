#pragma once
#include <Arduino.h>

using LimitCallback = void(*)();

class Button {

public:
    Button(uint8_t pin, bool activeLow, LimitCallback cb);
    void update();
    bool isPressed() const;

private:
    uint8_t pin;
    bool activeLow;
    bool triggered;
    unsigned long lastDebounceTime;
    bool lastReading;

    LimitCallback onPress;
    unsigned long debounceDelay = 50;
  };