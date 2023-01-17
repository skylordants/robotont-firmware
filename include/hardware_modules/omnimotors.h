#ifndef OMNIMOTORS_H
#define OMNIMOTORS_H

#include "hardware_modules/motor.h"

class OmniMotors {
public:
  OmniMotors();

  ~OmniMotors();

  Motor m[3];
};

#endif