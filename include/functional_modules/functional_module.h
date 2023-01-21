#ifndef FUNCTIONAL_MODULE_H
#define FUNCTIONAL_MODULE_H

#include <string>
#include <vector>
#include <map>
#include "mbed.h"

#include "hardware_modules/hardware_module.h"

class FunctionalModule
{
public:
  /*FunctionalModule(std::vector<std::string> packet_headers, std::vector<std::string> hardware_module_dependencies);*/

  FunctionalModule();

  virtual ~FunctionalModule();

  // TEMP:
  bool packetOwner(std::string packet_header);

  void registerDependency(std::string name);
  void resolveDependency(std::string name, HardwareModule *hardware_module);
  std::vector<std::string> getDependencies();
  bool dependenciesMet();

  /**
   * This function is meant to allow HardwareModule dependencies to be saved in multible variables to
   * reduce code complexity by not needing casts and calls to the map every time one is used.
   * And make other initializations that require any HardwareModules.
  */
  virtual bool startModule();

  void registerHeader(std::string packet_header);
  std::vector<std::string> ownedHeaders();

  void sendPacket(const char *fmt, ...);

  virtual void processPacket(const std::vector<std::string>& cmd) = 0;

  virtual void loop();

  virtual void stop();

protected:
  std::map<std::string, HardwareModule *> hardware_module_dependencies_;

private:
  std::vector<std::string> packet_headers_;
  std::vector<std::string> dependency_names_;
};

#endif