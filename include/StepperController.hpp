#include <AccelStepper.h>
#pragma once


class StepperController {
public:
    StepperController(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin);
    void init(int steps_Tot);
    void update(int steps_Tot);
    void handleMinTrigger(int steps_Tot);
    void handleMaxTrigger();
    void emergencyStop();
    void enable(int steps_Tot);
    void changeSpeedMode();

private:
    AccelStepper stepper;
    uint8_t enablePin;
    int stepsTot;
    bool movingToMin;
    int currentSpeedMode;
    int destinationMin;
    int destinationMax;

    void homingSequence();
};