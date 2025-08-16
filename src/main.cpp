#include <Arduino.h>
#include "StepperController.hpp"
#include "Button.hpp"
#include "Pins.hpp"

//Initial total steps
int steps_Tot = MAX_STEPS;
//State of the system: ON or OFF
volatile bool systemOn = false;
//Define the stepper controller with the pins and total steps
StepperController controller(STEP_PIN, DIR_PIN, ENABLE_PIN);

//My three buttons and both limit switches are active low

//Reset button. Does init action
Button resetButton(RESET_BUTTON_PIN, true, []() {
controller.init(steps_Tot);
});

//Power button to toggle the system state and enable/disable the motor
Button powerButton(POWER_BUTTON_PIN, true, []() {
    systemOn = !systemOn;
    if (systemOn) {
        controller.enable(steps_Tot);
    } else {
        controller.emergencyStop();
    }
});

//Speed mode button. Changes how the motor steps
Button changeSpeedButton(SPEED_MODE_PIN, true, []() {
    controller.changeSpeedMode();
});

//Minimum limit switch
Button minSwitch(LIMIT_MIN_PIN,true, []() {
    controller.handleMinTrigger(steps_Tot);
});

//Maximum limit switch
Button maxSwitch(LIMIT_MAX_PIN,true, []() {
    controller.handleMaxTrigger();
});


//Read the potentiometer and map its value to my range of steps
void checkPotentiometer() {
  // Read the potentiometer value
  int potValue = analogRead(POTENTIOMETER_PIN);
  // Map the potentiometer value to my range
  steps_Tot = map(potValue, 0, 1023, MAX_STEPS, MIN_STEPS);
}

//Initial setup
void setup() {
    //When the system starts, it is off
    systemOn = false;

    //Check the initial position of the potentiometer
    checkPotentiometer();

    //Initialize the stepper controller
    controller.init(steps_Tot);
    
    // Configure power button pin for interrupt
    pinMode(POWER_BUTTON_PIN, INPUT_PULLUP);
}

// Checks values of limit switches, buttons, potentiometer and updates the stepper controller
void loop() {
    powerButton.update();
    resetButton.update();
    checkPotentiometer();
    changeSpeedButton.update();
    //Just works when the system is ON
    if (systemOn) {
        minSwitch.update();
        maxSwitch.update();
        controller.update(steps_Tot);
    }
}
