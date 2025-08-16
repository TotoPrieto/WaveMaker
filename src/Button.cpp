#include "Button.hpp"

//Constructor. Parameters are: Pin from button, value when button is pressed, and what to do when pressed
Button::Button(uint8_t pin, bool activeLow, LimitCallback cb)
    : pin(pin), activeLow(activeLow), triggered(false),
      lastDebounceTime(0), onPress(cb){
    pinMode(pin, INPUT_PULLUP);
    lastReading = digitalRead(pin);
}

void Button::update() {
    //Read the current button state
    bool reading = digitalRead(pin);
    //Button NO or NC
    bool activeState = activeLow ? LOW : HIGH;

    //Update button value, update debounce time
    if (reading != lastReading) {
        lastDebounceTime = millis();
        lastReading = reading;
    }

    //Anti-rebound
    if ((millis() - lastDebounceTime) > debounceDelay) {
        //If the limit switch is pressed
        if (reading == activeState && !triggered) {
            triggered = true;
            if (onPress) onPress();
        }
        //Else button is released
        else if (reading != activeState) {
            triggered = false;
        }
    }
}

//Check if the button is pressed depending on button's resistance
bool Button::isPressed() const {
    bool reading = digitalRead(pin);
    return (activeLow ? (reading == LOW) : (reading == HIGH));
}
    