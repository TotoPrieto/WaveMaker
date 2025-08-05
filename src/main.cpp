#include <Arduino.h>
#include "StepperController.hpp"
#include "Button.hpp"
#include "Pins.hpp"


bool systemOn;
//Define the stepper controller with the pins and total steps
StepperController controller(STEP_PIN, DIR_PIN, ENABLE_PIN, STEPS_TOT);

// Define power button that toggles the system state when pressed. Mine is active low
Button powerButton(POWER_BUTTON_PIN, true, []() {
    systemOn = !systemOn;
});
// Define reset button. Mine is active high
Button resetButton(RESET_BUTTON_PIN, false, nullptr);

//Define minimum limit switch that toggle direction when pressed
Button minSwitch(LIMIT_MIN_PIN,true, []() {
    controller.handleMinTrigger();
});

// Define maximum limit switch that rehomes the zero position and toggles direction when pressed
Button maxSwitch(LIMIT_MAX_PIN,true, []() {
    controller.handleMaxTrigger();
});


void setup() {
    //When the system starts, it is off
    systemOn = false;
    controller.init();
}

void checkReset() {
  resetButton.update();
  if (resetButton.isPressed()) {
    controller.init();
  }
}


// Checks values of limit switches, buttons, and updates the stepper controller
void loop() {
    powerButton.update();
    checkReset();
    //Just works when the system is ON
    if (systemOn) {
      minSwitch.update();
      maxSwitch.update();  
      controller.update();
    }
}

