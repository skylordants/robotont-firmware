#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <map>
#include <vector>

#include "hardware_modules/hardware_module.h"
#include "functional_modules/functional_module.h"
#include "packetprocessor.h"

class Initializer
{
public:

  Initializer(PacketProcessor *packetprocessor); // I created the other constructor to stop this from being called when declaring the variable
  Initializer();
  ~Initializer();



  std::map<std::string, HardwareModule *> hardware_modules;
  std::vector<FunctionalModule *> functional_modules;

};

extern Initializer initializer; // In case we add some other functionality to the initalizer, which requires communication with it

#endif