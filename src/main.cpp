#include <Arduino.h>
#include "StepperController.hpp"
//#include "LimitSwitch.hpp"
#include "Button.hpp"
//#include "SystemController.hpp"
#include "Pins.hpp"

static const unsigned long debounce = 50;

StepperController controller(STEP_PIN, DIR_PIN, ENABLE_PIN, STEPS_TOT);

Button powerButton(POWER_BUTTON_PIN, false, debounce, nullptr);
Button resetButton(RESET_BUTTON_PIN, false, debounce, nullptr);
bool systemOn(false);

// Define limit switches with callbacks for handling triggers 

// When the minimum limit switch is pressed, the stepper will toggle direction
Button minSwitch(LIMIT_MIN_PIN,true, debounce, []() {
    controller.handleMinTrigger();
});

// When the maximum limit switch is pressed, the stepper will rehome the zero position and 
// toggle direction
Button maxSwitch(LIMIT_MAX_PIN,true, debounce, []() {
    controller.handleMaxTrigger();
});


void setup() {
    controller.init();
    //buttonController.init();
}

void checkPower() {
  if (powerButton.isPressed()) {
    // Toggle the system state
    systemOn = !systemOn;
    digitalWrite(ENABLE_PIN, systemOn ? LOW : HIGH);
  }
}

void checkReset() {
  if (resetButton.isPressed()) {
    controller.init();
  }
}

// Checks values of limit switches, buttons, and updates the stepper controller
void loop() {
    checkPower();
    checkReset();
    minSwitch.update();
    maxSwitch.update();
    if (systemOn) {

        controller.update();
    }
    //buttonController.update();
}

