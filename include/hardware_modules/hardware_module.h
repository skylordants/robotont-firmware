#ifndef HARDWARE_MODULE_H
#define HARDWARE_MODULE_H

#include <string>

class HardwareModule
{
public:
  std::string name;

  HardwareModule(std::string _name);
  ~HardwareModule();
};

#endif