#include <sstream>
#include <vector>
#include "packetprocessor.h"

#define MAX_CMD_ARGS 5

PacketProcessor::PacketProcessor(Timer* cmd_timer)
{
  cmd_timer_ = cmd_timer;
}

PacketProcessor::PacketProcessor()
{

}

void PacketProcessor::registerModule(FunctionalModule* funtional_module) {
  functional_modules_.push_back(funtional_module);
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

  for (int module = 0; module < functional_modules_.size(); module++) {
    if (functional_modules_[module]->packetOwner(cmd[0])) {
      functional_modules_[module]->processPacket(cmd, *cmd_timer_);
    }
  }

  /*if (motorModule->packetOwner(cmd[0]))
  {
    motorModule->processPacket(cmd, cmd_timer_);
  }*/

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
