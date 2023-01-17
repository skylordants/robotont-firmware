#ifndef PACKETPROCESSOR_H
#define PACKETPROCESSOR_H

#include <vector>

#include "mbed.h"
#include "functional_modules/functional_module.h"


class PacketProcessor
{
public:
  PacketProcessor(Timer* cmd_timer, RawSerial* serial_pc);

  PacketProcessor();

  void registerModule(FunctionalModule *functional_module);

  void processPacket(const std::string& packet);

  void sendPacket(char *fmt, ...);
private:
  Timer* cmd_timer_;
  RawSerial* serial_pc_;
  std::vector<FunctionalModule*> functional_modules_;
};

#endif