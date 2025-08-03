#include "LimitSwitch.hpp"
#include "Pins.hpp"


LimitSwitch::LimitSwitch(uint8_t pin, LimitCallback cb)
: pin(pin), lastState(HIGH), triggered(false), lastDebounceTime(0), onPress(cb) {
    pinMode(pin, INPUT_PULLUP);
}

void LimitSwitch::update() {
    bool reading = digitalRead(pin);

    //Update value limit switch, update debounce time
    if (reading != lastState) {
        lastDebounceTime = millis();
        lastState = reading;
    }
    //Anti-rebounce
    if ((millis() - lastDebounceTime) > debounceDelay) {
        //If the limit switch is pressed
        if (reading == LOW && !triggered) {
            triggered = true;
            if (onPress) onPress();
        //If the limit switch is released
        } else if (reading == HIGH) {
            triggered = false;
        }
    }
}