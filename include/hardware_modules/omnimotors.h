#ifndef OMNIMOTORS_H
#define OMNIMOTORS_H

#include "hardware_modules/motor.h"
#include "hardware_module.h"

class OmniMotors : public HardwareModule
{
public:
  OmniMotors(const MotorConfig& cfg0, const MotorConfig& cfg1, const MotorConfig& cfg2);

  ~OmniMotors();

  const MotorConfig motor_configs[3]; /**< Motor configurations */

  Motor m[3];
};

#endif