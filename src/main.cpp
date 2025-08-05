#include <Arduino.h>
#include "StepperController.hpp"
#include "Button.hpp"
#include "Pins.hpp"

int steps_Tot = MAX_STEPS;
bool systemOn;
//Define the stepper controller with the pins and total steps
StepperController controller(STEP_PIN, DIR_PIN, ENABLE_PIN);

//Define power button that toggles the system state when pressed. Mine is active low
Button powerButton(POWER_BUTTON_PIN, true, []() {
    systemOn = !systemOn;
});
//Define reset button. Mine is active high
Button resetButton(RESET_BUTTON_PIN, true, nullptr);

//Define minimum limit switch that toggle direction when pressed
Button minSwitch(LIMIT_MIN_PIN,true, []() {
    controller.handleMinTrigger();
});

//Define maximum limit switch that rehomes the zero position and toggles direction when pressed
Button maxSwitch(LIMIT_MAX_PIN,true, []() {
    controller.handleMaxTrigger(steps_Tot);
});

//Define speed mode button. Mine is active low
Button speedButton(SPEED_BUTTON_PIN, true, []() {
    controller.changeSpeedMode();
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

void checkPotentiometer() {
  // Read the potentiometer value
  int potValue = analogRead(POTENTIOMETER_PIN);
  // Map the potentiometer value to my range
  steps_Tot = map(potValue, 0, 1023, MAX_STEPS, MIN_STEPS);
}

// Checks values of limit switches, buttons, and updates the stepper controller
void loop() {
    powerButton.update();
    checkReset();
    speedButton.update();
    checkPotentiometer();
    //Just works when the system is ON
    if (systemOn) {
      minSwitch.update();
      maxSwitch.update();  
      controller.update(steps_Tot);
    }
}

