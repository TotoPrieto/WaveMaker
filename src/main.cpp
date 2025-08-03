#include <Arduino.h>
#include "StepperController.hpp"
#include "LimitSwitch.hpp"
#include "SystemController.hpp"
#include "Pins.hpp"


StepperController controller(STEP_PIN, DIR_PIN, ENABLE_PIN, STEPS_TOT);
SystemController buttonController;

// Define limit switches with callbacks for handling triggers 

// When the minimum limit switch is pressed, the stepper will toggle direction
LimitSwitch minSwitch(LIMIT_MIN_PIN, []() {
    controller.handleMinTrigger();
});

// When the maximum limit switch is pressed, the stepper will rehome the zero position and 
// toggle direction
LimitSwitch maxSwitch(LIMIT_MAX_PIN, []() {
    controller.handleMaxTrigger();
});

void setup() {
    controller.init();
    buttons.init();
}

// Checks values of limit switches, buttons, and updates the stepper controller
void loop() {
    buttonController.update();
    minSwitch.update(); 
    maxSwitch.update();
    controller.update();
}