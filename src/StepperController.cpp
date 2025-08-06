#include "StepperController.hpp"
#include "Pins.hpp"
#include <Arduino.h>

// int stepsTot
StepperController::StepperController(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin) 
: stepper(AccelStepper::DRIVER, stepPin, dirPin),
  enablePin(enablePin),
  movingToMin(true) {}

void StepperController::init() {
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);
    currentSpeedMode = 0;
    stepper.setMaxSpeed(1000);
    stepper.setAcceleration(4000);
    homingSequence();

    stepper.moveTo(-1550); // Cambien stepsTot por valor inicial
    movingToMin = true;
}

// Homing sequence to find the maximum position and position zero
void StepperController::homingSequence() {
    stepper.setSpeed(600);
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
void StepperController::rehomingSequence(int steps_Tot) {
    stepper.stop();
    // Move back so limit switch is no longer pressed
    stepper.move(-50);
    while (stepper.distanceToGo() != 0) stepper.run();
    // Set the current position as zero
    stepper.setCurrentPosition(0);
    stepper.moveTo(steps_Tot);
    movingToMin = true;
}

// In case minimum limit switch is pressed, stop and just move to zero position
void StepperController::handleMinTrigger() {
    stepper.stop();
    stepper.moveTo(0);
    movingToMin = false;
}

// In case the maximum limit switch is pressed, rehome zero position
void StepperController::handleMaxTrigger(int steps_Tot) {
    rehomingSequence(steps_Tot);
}

// Update the stepper position when stepper completes its movement
void StepperController::update(int steps_Tot) {
    if (stepper.distanceToGo() == 0) {
        stepper.moveTo(movingToMin ? 0 : steps_Tot);
        movingToMin = !movingToMin;
    }
    stepper.run();
}

// Change speed mode when the speed button is pressed
// There are 3 speed modes: slow, medium, and fast
void StepperController::changeSpeedMode() {
    currentSpeedMode = (currentSpeedMode + 1) % 3;
    switch (currentSpeedMode) {
        case 0:
            stepper.setMaxSpeed(1000);
            stepper.setAcceleration(4000);
            break;
        case 1:
            stepper.setMaxSpeed(1500);
            stepper.setAcceleration(4000);
            break;
        case 2:
            stepper.setMaxSpeed(2000);
            stepper.setAcceleration(4000);
            break;
    }
}

// Enable the stepper motor
void StepperController::enable(int steps_Tot) {
    //Enable the motor
    digitalWrite(enablePin, LOW);
    
    //Set the previous movement without changing direction when the motor is stopped
    if (stepper.distanceToGo() == 0) {
        stepper.moveTo(movingToMin ? steps_Tot : 0);
    }
}

// Disable the stepper motor for safety
void StepperController::emergencyStop() {
    // Stop any current movement immediately
    stepper.stop();
    
    // Force immediate stop
    stepper.setCurrentPosition(stepper.currentPosition());
    
    // Disable the motor
    digitalWrite(enablePin, HIGH); 
}