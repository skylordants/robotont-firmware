

#include "hardware_modules/omnimotors.h"
#include "motor_config.h"

OmniMotors::OmniMotors()
  : m({ { cfg0 }, { cfg1 }, { cfg2 } })
{

}

OmniMotors::~OmniMotors()
{
  
}