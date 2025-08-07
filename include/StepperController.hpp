#include <AccelStepper.h>
#pragma once


class StepperController {
public:

    // Constructor
    StepperController(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin);

    void init();
    void update(int steps_Tot);
    void handleMinTrigger(int steps_Tot);
    void handleMaxTrigger();
    void changeSpeedMode();
    void emergencyStop();
    void enable(int steps_Tot);
private:
    AccelStepper stepper;
    uint8_t enablePin;
    int stepsTot;
    bool movingToMin;
    int currentSpeedMode;

    void homingSequence();
    void rehomingSequence(int steps_Tot);
};