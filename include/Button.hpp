#pragma once
#include <Arduino.h>

using LimitCallback = void(*)();

class Button {
public:
    // Constructor: incluye callback opcional compartido para onPress y onRelease
    Button(uint8_t pin, bool activeLow, unsigned long debounce,LimitCallback cb);

    // Método para actualizar el estado del botón (debe llamarse en loop())
    void update();

    bool isPressed() const;

private:
    uint8_t pin;
    bool activeLow;
    bool triggered;
    unsigned long debounceDelay;
    unsigned long lastDebounceTime;
    bool lastReading;

    LimitCallback onPress;

  };