#include "StepperController.hpp"
#include "Pins.hpp"
#include <Arduino.h>

// int stepsTot
StepperController::StepperController(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin) 
: stepper(AccelStepper::DRIVER, stepPin, dirPin),
  enablePin(enablePin),
  movingToMin(true) {}

void StepperController::init(int steps_Tot) {
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);
    currentSpeedMode = 0;
    stepper.setMaxSpeed(5000);
    stepper.setAcceleration(2000);
    homingSequence();
    stepper.setAcceleration(3000);
    stepper.moveTo(2000); // Cambien stepsTot por valor inicial
}

// Homing sequence to find the minimum position and position zero
void StepperController::homingSequence() {
    //stepper.setSpeed(-1500);
    stepper.setCurrentPosition(0);

    stepper.moveTo(-2000);
    //int initialSpeed = stepper.speed();
    //int acceleration = stepper.acceleration();
    //stepper.setMaxSpeed(2000);
    //stepper.setAcceleration(2000);
    // Move to the minimum position until the limit switch is pressed
    while (digitalRead(LIMIT_MIN_PIN) == HIGH) {
        stepper.run();
    }

    stepper.stop();
    delayMicroseconds(5000);
    // Move back so limit switch is no longer pressed
    stepper.setCurrentPosition(0);
    movingToMin = false;
    //stepper.move(200);
    //stepper.runToPosition();
    // Set the current position as zero
    //stepper.setCurrentPosition(0);
    //stepper.setMaxSpeed(initialSpeed);
    //stepper.setAcceleration(acceleration);
}

// Rehoming sequence
void StepperController::rehomingSequence(int steps_Tot) {
    // Move back so limit switch is no longer pressed
    stepper.setCurrentPosition(0);
//    stepper.move(200);
//    stepper.runToPosition();
    // Set the current position as zero
//    stepper.setCurrentPosition(0);
    stepper.moveTo(steps_Tot);
//    movingToMin = true;
}

// In case minimum limit switch is pressed, stop and just move to zero position
void StepperController::handleMinTrigger(int steps_Tot) {
    stepper.setSpeed(0);
    stepper.stop();
    delayMicroseconds(5000);
    rehomingSequence(steps_Tot);

    movingToMin = false;
}

// In case the maximum limit switch is pressed, rehome zero position
void StepperController::handleMaxTrigger(int steps_Tot) {
    stepper.setSpeed(0);
    stepper.stop();
    delayMicroseconds(5000);
    homingSequence();
    stepper.moveTo(2000);
}

// Update the stepper position when stepper completes its movement
void StepperController::update(int steps_Tot) {    
    //stepper.setSpeed(1000); // Set a constant speed for the stepper
    //stepper.runSpeedToPosition();
    //stepper.run();
    if (movingToMin) {
        stepper.run();
    }else{
        stepper.setSpeed(1000);
        stepper.moveTo(steps_Tot);
        stepper.runToPosition();
        stepper.setSpeed(0);
        delayMicroseconds(5000);
        stepper.stop();
        stepper.moveTo(-50);
        movingToMin = true;
    }
}

// Change speed mode when the speed button is pressed
// There are 3 speed modes: slow, medium, and fast
void StepperController::changeSpeedMode() {
    currentSpeedMode = (currentSpeedMode + 1) % 4;
    switch (currentSpeedMode) {
        case 0:
            // Para RPM=600
            stepper.setMaxSpeed(5000);
            stepper.setAcceleration(3000);
            break;
        case 1:
            // Ya probado y no funciona
            stepper.setMaxSpeed(5000);
            stepper.setAcceleration(3500);
            break;
        case 2:
            // Nunca probado
            stepper.setMaxSpeed(7000);
            stepper.setAcceleration(3000);
            break;
        case 3:
            // Nunca probado
            stepper.setMaxSpeed(9000);
            stepper.setAcceleration(3000);
            break;
    }
}

// Enable the stepper motor
void StepperController::enable(int steps_Tot) {
    //Enable the motor
    digitalWrite(enablePin, LOW);
    
    //Set the previous movement without changing direction when the motor is stopped
    if (stepper.distanceToGo() == 0) {
        stepper.moveTo(movingToMin ? 0 : steps_Tot);
    }
}

// Disable the stepper motor for safety
void StepperController::emergencyStop() {
    // Stop any current movement immediately
    //stepper.setSpeed(0);
    stepper.stop();
    
    // Force immediate stop
    stepper.setCurrentPosition(stepper.currentPosition());
    
    // Disable the motor
    digitalWrite(enablePin, HIGH); 
}