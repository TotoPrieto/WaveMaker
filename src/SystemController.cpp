#include "SystemController.hpp"
#include "StepperController.hpp"
#include "Pins.hpp"
#include <Arduino.h>

SystemController::SystemController()
  : stepper(STEP_PIN, DIR_PIN, ENABLE_PIN, STEPS_TOT),
    powerButton(POWER_BUTTON_PIN, false, debounce, nullptr), 
    resetButton(RESET_BUTTON_PIN, false, debounce, nullptr),
    systemOn(false) {}

void SystemController::init() {
  stepper.init();

}

void SystemController::update() {
  checkPower();
  checkReset();

  if (systemOn) {
    stepper.update();
  }
}

void SystemController::checkPower() {
  if (powerButton.isPressed()) {
    // Toggle the system state
    systemOn = !systemOn;
    digitalWrite(ENABLE_PIN, systemOn ? LOW : HIGH);
  }
}

void SystemController::checkReset() {
  if (resetButton.isPressed()) {
    stepper.init();
  }
}