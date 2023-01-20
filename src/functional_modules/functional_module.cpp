#include "functional_modules/functional_module.h"
#include "packetprocessor.h"

FunctionalModule::FunctionalModule(/*std::vector<std::string> packet_headers*/)
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