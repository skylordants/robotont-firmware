#include "mbed.h"
#include "hardware_modules/motor.h"
#include "functional_modules/odom.h"
#include <sstream>
#include <vector>
#include "motor_config.h"


#include "motor_config.h"
#include "packetprocessor.h"

// Modules
#include "hardware_modules/omnimotors.h"
#include "functional_modules/omnimotors_control.h"


// Temp initializations: eventually somehow automatically
OmniMotors omnimotors = OmniMotors();
OmniMotorsControl* motorModule;
PacketProcessor packetprocessor;

// Initialize odometry
Odom odom_(cfg0, cfg1, cfg2, MAIN_DELTA_T);

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

    // if escape is received, clear the buffer and stop the motors for now
    if (c == 27)  // esc
    {
      motorModule->stop();

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
  motorModule = new OmniMotorsControl(omnimotors);
  packetprocessor = PacketProcessor(&serial_pc);

  packetprocessor.registerModule(motorModule);


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
    
    // Update odometry
    odom_.update(omnimotors.m[0].getMeasuredSpeed(), omnimotors.m[1].getMeasuredSpeed(), omnimotors.m[2].getMeasuredSpeed());
    serial_pc.printf("ODOM:%f:%f:%f:%f:%f:%f\r\n", odom_.getPosX(), odom_.getPosY(),
                     odom_.getOriZ(), odom_.getLinVelX(), odom_.getLinVelY(), odom_.getAngVelZ());
    // Synchronize to given MAIN_DELTA_T
    wait_us(MAIN_DELTA_T*1000*1000 - main_timer.read_us());
  }
}
