#pragma once
#include "StepperController.hpp"
#include "LimitSwitch.hpp"
#include "Button.hpp"

class SystemController {
public:
  SystemController();
  void init();
  void update();

private:
  StepperController stepper;
  LimitSwitch minSwitch;
  LimitSwitch maxSwitch;
  Button powerButton;
  Button resetButton;

  bool systemOn;

  void checkPower();
  void checkReset();
};