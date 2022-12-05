#ifndef FUNCTIONAL_MODULE_H
#define FUNCTIONAL_MODULE_H

#include <string>
#include <vector>
#include "mbed.h"

class FunctionalModule
{
public:
  FunctionalModule(std::vector<std::string> packet_headers);

  bool packetOwner(std::string packet_header);

  virtual void processPacket(const std::vector<std::string>& cmd, Timer& cmd_timer) = 0;

  virtual void stop();

protected:
  std::vector<std::string> packet_headers_;
};

#endif