#include <AccelStepper.h>
#pragma once


class StepperController {
public:
    StepperController(uint8_t stepPin, uint8_t dirPin, uint8_t enablePin, int stepsTot);

    void init();
    void update();
    void handleMinTrigger();
    void handleMaxTrigger();

private:
    AccelStepper stepper;
    uint8_t enablePin;
    int stepsTot;
    bool movingToMin;

    void homingSequence();
    void rehomingSequence();
};