#include "StepperController.hpp"
#include "Pins.hpp"
#include <Arduino.h>

// Constructor
StepperController::StepperController(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin) 
: stepper(AccelStepper::DRIVER, stepPin, dirPin),
  enablePin(enablePin),
  movingToMin(true) {}

// Initialize the stepper motor
void StepperController::init(int steps_Tot) {
    pinMode(enablePin, OUTPUT);
    digitalWrite(enablePin, LOW);
    //Values for step with acceleration
    stepper.setMaxSpeed(5000);
    stepper.setAcceleration(3000);

    homingSequence();
    //Values for step with constant speed
    stepper.setSpeed(1000);

    currentSpeedMode = 0;
}

// Homing sequence to find the minimum position and declare the zero
void StepperController::homingSequence() {

    // Move to the minimum position until the limit switch is pressed
    stepper.setCurrentPosition(0);
    stepper.moveTo(-2000);
    while (digitalRead(LIMIT_MIN_PIN) == HIGH) {
        stepper.run();
    }
    stepper.stop();
    stepper.setCurrentPosition(0);
    //Change direction to positive
    movingToMin = false;
    stepper.moveTo(2000);
    
}


// In case minimum limit switch is pressed then stop, declare zero position and set destination
void StepperController::handleMinTrigger(int steps_Tot) {
    // Set the destination based on the current speed mode
    switch(currentSpeedMode){
        case 0:
            //The destination is defined with the potenciometer
            destinationMin = steps_Tot;
            break;
        case 1: 
            //Much higher than MAX_STEPS in order to ensure it reaches the limit
            destinationMin = 2200;
            break;
        case 2:
            //Much higher than MAX_STEPS in order to ensure it reaches the limit
            destinationMin = 2200;
            break;
    }

    //"IF sentence" to avoid problems
    if (stepper.distanceToGo() != 0) {
        stepper.stop();
        stepper.setCurrentPosition(0);
        stepper.moveTo(destinationMin);
        movingToMin = false;
    }
}

// In case the maximum limit switch is pressed then stop, declare zero position(helps to stop suddenly) and set destination
void StepperController::handleMaxTrigger() {
    //"IF sentence" to avoid problems
    if (stepper.distanceToGo() != 0) {
        stepper.stop();
        stepper.setCurrentPosition(0);
        //Much lower than zero in order to ensure it reaches the limit
        stepper.moveTo(-2200);
        movingToMin = true;
    }
}


//Update the stepper position when stepper completes its movement
void StepperController::update(int steps_Tot) {
    switch(currentSpeedMode) {
 
        case 0:
        //This mode changes the speed depending the destination. The range doesn't touch de limit switch
            //If moving to -Z, moves with acceleration
            if (movingToMin) {
                // Move towards the minimum position with acceleration
                stepper.run();
                // If the stepper has reached the destination, change direction manually
                if (stepper.distanceToGo() == 0) {
                    stepper.stop();
                    movingToMin = false;
                    // Sets the next position to go that doesn't touch the limit switch
                    stepper.moveTo(steps_Tot);
                    // Sets values for the other direction
                    stepper.setSpeed(1000);
                }
            }else{
                //If moving to +Z, moves with constant speed (no acceleration)
                stepper.runSpeedToPosition();
                if (stepper.distanceToGo() == 0) {
                    stepper.stop();
                    movingToMin = true;
                    // Sets the next position to go that doesn't touch the limit switch
                    stepper.moveTo(100);
                    // Sets values for the other direction
                    stepper.setMaxSpeed(9000);
                    stepper.setAcceleration(3000);
            }
            break;

        case 1:
        //This mode goes from limit switch to limit switch at constant speed
            stepper.setSpeed(1000);
            stepper.runSpeedToPosition();
            break;

        case 2:
        //This mode goes from limit switch to limit switch with acceleration/deceleration
            stepper.run();
            break;
        }
    }
}

// Enable the stepper motor
void StepperController::enable(int steps_Tot) {

    // Checks the actual destination depending the mode to avoid changing it unexpectedly
    switch(currentSpeedMode){
        case 0:
            destinationMin = steps_Tot;
            break;
        case 1: 
            destinationMin = 2200;
            break;
        case 2:
            destinationMin = 2200;
            break;
    }
    //Enable the motor
    digitalWrite(enablePin, LOW);
    
    //Set the previous movement without changing direction when the motor is stopped
    if (stepper.distanceToGo() == 0) {
        stepper.moveTo(movingToMin ? 0 : destinationMin);
    }
}

// Disable the stepper motor
void StepperController::emergencyStop() {
    // Stop any current movement immediately
    stepper.setSpeed(0);
    stepper.stop();
    
    // Force immediate stop
    stepper.setCurrentPosition(stepper.currentPosition());
    
    // Disable the motor
    digitalWrite(enablePin, HIGH); 
}

// Change the speed mode and perform homing if necessary to avoid problems
void StepperController::changeSpeedMode() {
    currentSpeedMode = (currentSpeedMode + 1) % 3;
    stepper.stop();
        //Do homing in case sistem is ON
    if (digitalRead(enablePin) == LOW) {
        homingSequence();
    }
    //Set all values of speed
    stepper.setSpeed(1000);
    stepper.setMaxSpeed(3000);
    stepper.setAcceleration(2000);
}