#ifndef FUNCTIONAL_MODULE_H
#define FUNCTIONAL_MODULE_H

#include <string>
#include <vector>
#include "mbed.h"

class FunctionalModule
{
public:
  FunctionalModule(/*std::vector<std::string> packet_headers !!!Until I figure out, how to do this!!!*/);

  virtual ~FunctionalModule();

  // TEMP:
  bool packetOwner(std::string packet_header);

  void registerHeader(std::string packet_header);

  std::vector<std::string> ownedHeaders();

  virtual void processPacket(const std::vector<std::string>& cmd) = 0;

  virtual void loop();

  virtual void stop();

private:
  std::vector<std::string> packet_headers_;
};

#endif