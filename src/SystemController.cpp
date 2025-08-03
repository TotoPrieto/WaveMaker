#include "SystemController.hpp"
#include "Pins.hpp"
#include <Arduino.h>

SystemController::SystemController()
  : stepper(STEP_PIN, DIR_PIN, ENABLE_PIN, STEPS_TOT, LIMIT_MIN_PIN, LIMIT_MAX_PIN),
    minSwitch(LIMIT_MIN_PIN), maxSwitch(LIMIT_MAX_PIN),
    powerButton(POWER_BUTTON_PIN), resetButton(RESET_BUTTON_PIN),
    systemOn(false) {}

void SystemController::init() {
  powerButton.init();
  resetButton.init();
  stepper.init();
  minSwitch.init();
  maxSwitch.init();
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