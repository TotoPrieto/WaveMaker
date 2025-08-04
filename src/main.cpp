#include <Arduino.h>
#include "StepperController.hpp"
#include "Button.hpp"
#include "Pins.hpp"

static const unsigned long debounce = 50;

bool systemOn;

StepperController controller(STEP_PIN, DIR_PIN, ENABLE_PIN, STEPS_TOT);

Button powerButton(POWER_BUTTON_PIN, true, debounce, []() {
    systemOn = !systemOn;
});
Button resetButton(RESET_BUTTON_PIN, true, debounce, nullptr);

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
    systemOn = false;
    controller.init();
}

/*
void checkPower() {
    powerButton.update();
  if (powerButton.isPressed()) {
    // Toggle the system state
    systemOn = !systemOn;
  }
}
  */

void checkReset() {
  resetButton.update();
  if (resetButton.isPressed()) {
    controller.init();
  }
}


// Checks values of limit switches, buttons, and updates the stepper controller
void loop() {
  //  checkPower();
    powerButton.update();
    checkReset();
    //Just work when the system is on
    if (systemOn) {
      minSwitch.update();
      maxSwitch.update();  
      controller.update();
    }
}

