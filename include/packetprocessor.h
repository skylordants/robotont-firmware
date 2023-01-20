#ifndef PACKETPROCESSOR_H
#define PACKETPROCESSOR_H

#include <vector>
#include <map>

#include "mbed.h"
#include "functional_modules/functional_module.h"


class PacketProcessor
{
public:
  PacketProcessor(RawSerial *serial_pc);

  PacketProcessor();

  void registerHeader(const std::string &header, FunctionalModule *functional_module);

  void processPacket(const std::string &packet);

  void sendPacket(const char *buffer);

private:
  RawSerial *serial_pc_;
  std::map <std::string, FunctionalModule *> header_map_;
};

// For FunctionalModules to be able to send packets and not create a cyclic dependancy
extern PacketProcessor packetprocessor;

#endif