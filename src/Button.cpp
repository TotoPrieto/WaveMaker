#include "Button.hpp"


Button::Button(uint8_t pin, bool activeLow, unsigned long debounce)
    : pin(pin), activeLow(activeLow), triggered(false), debounceDelay(debounce),
      lastDebounceTime(0), onPress(cb), onRelease(cb) {
    pinMode(pin, INPUT_PULLUP); // para ambos tipos
    lastReading = digitalRead(pin);
}



void Button::update() {
    bool reading = digitalRead(pin);
    bool activeState = activeLow ? LOW : HIGH;

    if (reading != lastReading) {
        lastDebounceTime = millis();
        lastReading = reading;
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading == activeState && !triggered) {
            triggered = true;
            if (onPress) onPress();
        } else if (reading != activeState && triggered) {
            triggered = false;
            if (onRelease) onRelease();
        }
    }
}

/*
bool Button::isPressed() const {
    bool reading = digitalRead(pin);
    return (activeLow ? (reading == LOW) : (reading == HIGH));
}
    */