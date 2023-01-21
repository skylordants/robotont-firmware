

#include "hardware_modules/omnimotors.h"
#include "motor_config.h"

OmniMotors::OmniMotors(const MotorConfig& cfg0, const MotorConfig& cfg1, const MotorConfig& cfg2)
  : HardwareModule("OmniMotors")
  , motor_configs({ cfg0, cfg1, cfg2 })
  , m({ { cfg0 }, { cfg1 }, { cfg2 } })
{

}

OmniMotors::~OmniMotors()
{
  
}