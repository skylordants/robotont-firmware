#include "initializer.h"

// Here can desired HardwareModules, FunctionalModule and other necessary headers be included

// HardwareModules:
#include "hardware_modules/omnimotors.h"
#include "motor_config.h"

// FunctionalModules:
#include "functional_modules/odom.h"
#include "functional_modules/omnimotors_control.h"


Initializer::Initializer(PacketProcessor *packetprocessor)
{
  // HardwareModules initialization with necessary parameters
  hardware_modules["OmniMotors"] = new OmniMotors(cfg0, cfg1, cfg2);
  

  // FunctionalModules initialization
  functional_modules.push_back(new OmniMotorsControl());
  functional_modules.push_back(new Odom());


  for (std::vector<FunctionalModule *>::iterator module = functional_modules.begin(); module != functional_modules.end(); module++) {
    // Register all headers
    std::vector<std::string> headers = (*module)->ownedHeaders();
    for (std::vector<std::string>::iterator header = headers.begin(); header != headers.end(); header++) {
      packetprocessor->registerHeader(*header, *module);
    }

    // Resolve all dependencies
    std::vector<std::string> dependencies = (*module)->getDependencies();
    for (std::vector<std::string>::iterator dependency = dependencies.begin(); dependency != dependencies.end(); dependency++) {
      if (hardware_modules.find(*dependency) != hardware_modules.end())
      {
        (*module)->resolveDependency(*dependency, hardware_modules[*dependency]);
      }
      else
      {
        // Some potential error management or don't start the module?
      }
    }

    (*module)->startModule();
  }
}

Initializer::Initializer()
{

}

Initializer::~Initializer()
{

}