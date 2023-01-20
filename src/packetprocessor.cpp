#include <sstream>
#include <vector>
#include <cstdarg>

#include "packetprocessor.h"

#define MAX_CMD_ARGS 5

PacketProcessor::PacketProcessor(RawSerial *serial_pc)
  : serial_pc_(serial_pc)
{

}

PacketProcessor::PacketProcessor()
{

}

void PacketProcessor::registerHeader(const std::string &header, FunctionalModule *functional_module)
{
  header_map_[header] = functional_module;
}

// This method processes a received serial packet
void PacketProcessor::processPacket(const std::string& packet)
{
  std::istringstream ss(packet);
  std::string arg;
  std::vector<std::string> cmd;

  for (int i = 0; i <= MAX_CMD_ARGS; i++)
  {
    arg.clear();
    std::getline(ss, arg, ':');
    if (arg.length())
    {
      cmd.push_back(arg);
      //serial_pc.printf("Got arg %s\r\n", arg.c_str());
    }
    else
    {
      break;
    }
  }

  if (!cmd.size())
  {
    return;
  }

  if (header_map_.find(cmd[0]) != header_map_.end())
  {
    header_map_[cmd[0]]->processPacket(cmd);
  }


  /*else if (cmd[0] == "PID")  // Update PID parameters
  {
    float k_p = 0.0f;
    float tau_i = 0.0f;
    float tau_d = 0.0f;
    // sscanf(ss.str().c_str(), "%f:%f:%f", &k_p, &tau_i, &tau_d);
    // for (uint8_t i = 0; i < 3; i++)
    //{
    //  m[i].setPIDTunings(k_p, tau_i, tau_d);
    //}
  }*/
}

void PacketProcessor::sendPacket(const char *buffer)
{
  serial_pc_->printf("%s", buffer);
}
