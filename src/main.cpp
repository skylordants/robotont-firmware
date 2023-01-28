#include "mbed.h"
#include <sstream>
#include <vector>
#include <map>

#include "motor_config.h"
#include "packetprocessor.h"
#include "initializer.h"


PacketProcessor packetprocessor;  // This must be a global variable for FunctionalModules
Initializer initializer;

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
      for (std::vector<FunctionalModule *>::iterator module = initializer.functional_modules.begin(); module != initializer.functional_modules.end(); module++)
      {
        (*module)->stop();
      }

      serial_buf[0] = '\0';
      serial_arrived = 0;
    }
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
  initializer = Initializer(&packetprocessor);

  // MAIN LOOP
  while (true)
  {
    main_timer.reset();
    main_timer.start();

    if (packet_received_b) // packet was completeted with \r \n
    {
      std::string packet(serial_buf);
      serial_buf[0] = '\0';
      serial_arrived = 0;
      packetprocessor.processPacket(packet);
      packet_received_b = false;
    }

    // Synchronize to given MAIN_DELTA_T
    wait_us(MAIN_DELTA_T*1000*1000 - main_timer.read_us());
  }
}
