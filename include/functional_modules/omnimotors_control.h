#ifndef MOTOR_MODULE_H
#define MOTOR_MODULE_H

#include "functional_modules/functional_module.h"
#include "motor_config.h"
#include "hardware_modules/omnimotors.h"

#define MOTOR_COUNT 3


class OmniMotorsControl : public FunctionalModule
{
public:
  OmniMotorsControl(OmniMotors *omnimotors);

  ~OmniMotorsControl();

  void processPacket(const std::vector<std::string>& cmd);

  void stop();

private:
  void checkForTimeout();
  Timer cmd_timer_;
  Ticker cmd_timeout_checker_;

  OmniMotors *omnimotors_;
};

#endif