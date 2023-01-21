#include "mbed.h"
#include <sstream>
#include <vector>
#include <map>
#include "motor_config.h"

#include "packetprocessor.h"

// Modules
#include "hardware_modules/hardware_module.h"
#include "hardware_modules/motor.h"
#include "hardware_modules/omnimotors.h"
#include "functional_modules/functional_module.h"
#include "functional_modules/odom.h"
#include "functional_modules/omnimotors_control.h"
#include "motor_config.h"

// Temp declarations: eventually somehow automatically
PacketProcessor packetprocessor;  // This must be a global variable for FunctionalModules
std::map<std::string, HardwareModule *> hardware_modules;
std::vector<FunctionalModule *> functional_modules;


// Timeout
Timer main_timer;

// Variables for serial connection
RawSerial serial_pc(USBTX, USBRX);  // tx, rx
char serial_buf[256];        // Buffer for incoming serial data
volatile uint8_t serial_arrived = 0;  // Number of bytes arrived
volatile bool packet_received_b = false;

// Process an incoming serial byte
void pc_rx_callback()
{
  // Store bytes from serial in our buffer until packet
  // termination byte 'enter', '\n', '\r' etc has arrived
  while (serial_pc.readable())
  {
    char c = serial_pc.getc();
    serial_buf[serial_arrived++] = c;
    serial_buf[serial_arrived] = '\0';
    if (serial_arrived > 254)
    {
      serial_arrived = 0;
    }

    if (c == '\n' || c == '\r')  // command terminated
    {
      if (serial_arrived > 3)
      {
        // signal that the packet is complete for processing
	      packet_received_b = true;
      }

    }

    // if escape is received, clear the buffer and stop the motors for now / changed to stopping all modules currently
    if (c == 27)  // esc
    {
      for (std::vector<FunctionalModule *>::iterator module = functional_modules.begin(); module != functional_modules.end(); module++)
      {
        (*module)->stop();
      }

      serial_buf[0] = '\0';
      serial_arrived = 0;
    }
  }
}

// All the module initializations, currently only FunctionalModules, but in the end, also HardwareModules
void init()
{
  hardware_modules["OmniMotors"] = new OmniMotors(cfg0, cfg1, cfg2);

  functional_modules.push_back(new OmniMotorsControl());
  functional_modules.push_back(new Odom());

  for (std::vector<FunctionalModule *>::iterator module = functional_modules.begin(); module != functional_modules.end(); module++) {
    // Register all headers
    std::vector<std::string> headers = (*module)->ownedHeaders();
    for (std::vector<std::string>::iterator header = headers.begin(); header != headers.end(); header++) {
      packetprocessor.registerHeader(*header, *module);
    }

    // Resolve all dependencies
    std::vector<std::string> dependencies = (*module)->getDependencies();
    for (std::vector<std::string>::iterator dependency = dependencies.begin(); dependency != dependencies.end(); dependency++) {
      if (hardware_modules.find(*dependency) != hardware_modules.end())
      {
        (*module)->resolveDependency(*dependency, hardware_modules[*dependency]);
      }
      else
      {
        // Some potential error management or don't start the module?
      }
    }

    (*module)->startModule();
  }
}

int main()
{
  // Initialize serial connection
  serial_pc.baud(115200);
  serial_buf[0] = '\0';
  serial_pc.attach(&pc_rx_callback);
  serial_pc.printf("**** MAIN ****\r\n");

  // Initialize modules
  packetprocessor = PacketProcessor(&serial_pc);

  init();



  // MAIN LOOP
  while (true)
  {
    main_timer.reset();
    main_timer.start();
    /*for (uint8_t i = 0; i < MOTOR_COUNT; i++)  // Possibly debug for modules?
    {
      // MOTOR DEBUG
      // serial_pc.printf("\r\n");
//      serial_pc.printf("MOTOR %d: \r\n", i);
//      serial_pc.printf("Speed[%d]: %f (%f): \r\n", i, m[i].getMeasuredSpeed(),
//                       m[i].getSpeedSetPoint());
//      // serial_pc.printf("Effort: %f: \r\n", m[i].getEffort());
//      serial_pc.printf("Fault: %u: \r\n", m[i].getFaultPulseCount());
//      serial_pc.printf("Current[%d]: %f: \r\n", i, m[i].getCurrent());
    }*/

//    serial_pc.printf("Serial arrived: %d\r\n", serial_arrived);
    
    if (packet_received_b) // packet was completeted with \r \n
    {
      std::string packet(serial_buf);
      serial_buf[0] = '\0';
      serial_arrived = 0;
      packetprocessor.processPacket(packet);
      packet_received_b = false;
    }
    
    // Call all loops, currently only Update odometry
    for (std::vector<FunctionalModule *>::iterator module = functional_modules.begin(); module != functional_modules.end(); module++)
    {
      (*module)->loop();
    }

    // Synchronize to given MAIN_DELTA_T
    wait_us(MAIN_DELTA_T*1000*1000 - main_timer.read_us());
  }
}
