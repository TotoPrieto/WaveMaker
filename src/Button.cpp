#include "Button.hpp"

//Constructor
//Pin from button, value when button is pressed, debounce time, and what to do when pressed
Button::Button(uint8_t pin, bool activeLow, unsigned long debounce, LimitCallback cb)
    : pin(pin), activeLow(activeLow), triggered(false), debounceDelay(debounce),
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

    //Anti-rebounce
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


bool Button::isPressed() const {
    bool reading = digitalRead(pin);
    return (activeLow ? (reading == LOW) : (reading == HIGH));
}
    