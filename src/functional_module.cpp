#include "functional_module.h"

FunctionalModule::FunctionalModule(std::vector<std::string> packet_headers)
{

}

bool FunctionalModule::packetOwner(std::string packet_header)
{
  for (int i = 0; i < packet_headers_.size(); i++) {
    if (packet_headers_[i] == packet_header) {
      return true;
    }
  }
	return false;
}

void FunctionalModule::stop() {

}