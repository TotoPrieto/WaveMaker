#include "StepperController.hpp"
#include "Pins.hpp"
#include <Arduino.h>

StepperController::StepperController(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin, int stepsTot)
: stepper(AccelStepper::DRIVER, stepPin, dirPin),
  enablePin(enablePin),
  stepsTot(stepsTot),
  movingToMin(true) {}

void StepperController::init() {
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);

    stepper.setMaxSpeed(300);
    stepper.setAcceleration(2000);
    homingSequence();

    stepper.moveTo(stepsTot);
    movingToMin = true;
}

// Homing sequence to find the maximum position and position zero
void StepperController::homingSequence() {
    stepper.setSpeed(150);
    // Move to the maximum position until the limit switch is pressed
    while (digitalRead(LIMIT_MAX_PIN) == HIGH) {
        stepper.runSpeed();
    }
    stepper.setSpeed(0);
    // Move back so limit switch is no longer pressed
    stepper.move(-50);
    while (stepper.distanceToGo() != 0) stepper.run();
    // Set the current position as zero
    stepper.setCurrentPosition(0);
    stepper.setSpeed(300);
}

// Rehoming sequence
void StepperController::rehomingSequence() {
    stepper.stop();
    // Move back so limit switch is no longer pressed
    stepper.move(-50);
    while (stepper.distanceToGo() != 0) stepper.run();
    // Set the current position as zero
    stepper.setCurrentPosition(0);
    stepper.moveTo(stepsTot);
    movingToMin = true;
}

// In case minimum limit switch is pressed, stop and just move to zero position
void StepperController::handleMinTrigger() {
    stepper.stop();
    stepper.moveTo(0);
    movingToMin = false;
}

// In case the maximum limit switch is pressed, rehome zero position
void StepperController::handleMaxTrigger() {
    rehomingSequence();
}

// Update the stepper position when stepper completes its movement
void StepperController::update() {
    if (stepper.distanceToGo() == 0) {
        stepper.moveTo(movingToMin ? 0 : stepsTot);
        movingToMin = !movingToMin;
    }
    stepper.run();
}