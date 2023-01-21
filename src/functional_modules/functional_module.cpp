#include "functional_modules/functional_module.h"
#include "packetprocessor.h"

/*FunctionalModule::FunctionalModule(std::vector<std::string> packet_headers, std::vector<std::string> hardware_module_dependencies)
  : hardware_module_dependencies_(hardware_module_dependencies)
  , packet_headers_(packet_headers)
{
  
}*/

FunctionalModule::FunctionalModule()
{

}

FunctionalModule::~FunctionalModule()
{

}

bool FunctionalModule::packetOwner(std::string packet_header)
{
  for (unsigned int i = 0; i < packet_headers_.size(); i++) {
    if (packet_headers_[i] == packet_header) {
      return true;
    }
  }
	return false;
}

void FunctionalModule::registerDependency(std::string name)
{
  if (hardware_module_dependencies_.find(name) == hardware_module_dependencies_.end())
  {
    hardware_module_dependencies_[name] = NULL;
  }
}

void FunctionalModule::resolveDependency(std::string name, HardwareModule *hardware_module)
{
  if (hardware_module_dependencies_.find(name) != hardware_module_dependencies_.end())
  {
    hardware_module_dependencies_[name] = hardware_module;
  }
}

std::vector<std::string> FunctionalModule::getDependencies()
{
  return dependency_names_;
}

bool FunctionalModule::dependenciesMet()
{
  for (int module = 0; module < dependency_names_.size(); module++)
  {
    if (hardware_module_dependencies_[dependency_names_[module]] == NULL)
    {
      return false;
    }
  }
  return true;
}

bool FunctionalModule::startModule()
{
  if (!dependenciesMet())
  {
    return false;
  }

  return true;
}

void FunctionalModule::registerHeader(std::string packet_header)
{
  packet_headers_.push_back(packet_header);
}

std::vector<std::string> FunctionalModule::ownedHeaders()
{
  return packet_headers_;
}

void FunctionalModule::loop()
{

}

void FunctionalModule::stop()
{

}

void FunctionalModule::sendPacket(const char *fmt, ...) {
  char buffer[1000] = {0};

  std::va_list args;

  va_start(args, fmt);
  vsnprintf(buffer, 1000, fmt, args);
  va_end(args);
  packetprocessor.sendPacket(buffer);
}