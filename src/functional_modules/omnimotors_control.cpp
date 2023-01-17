#include <vector>
#include <string.h>

#include "functional_modules/omnimotors_control.h"

//std::vector<std::string> headers("MS", "RS");

OmniMotorsControl::OmniMotorsControl(OmniMotors omnimotors_)
  /*: FunctionalModule ((std::vector<std::string>){"MS", "RS"})*/
{
  registerHeader("MS");
  registerHeader("RS");
  omnimotors = omnimotors_;
}

OmniMotorsControl::~OmniMotorsControl()
{

}

void OmniMotorsControl::processPacket(const std::vector<std::string>& cmd, Timer& cmd_timer) {
  // MS - Set motor speeds manually (linear speed on wheel m/s)
  /* MS:motor1_speed:motor2_speed:motor3_speed */
  if (cmd[0] == "MS")
  {
    for (uint8_t i = 0; i < MOTOR_COUNT; i++)
    {
      float speed_setpoint = std::atof(cmd[i + 1].c_str());
      //serial_pc.printf("Setpoint %d, %f\r\n", i, speed_setpoint);
      omnimotors.m[i].setSpeedSetPoint(speed_setpoint);
    }
    cmd_timer.reset();
  }

  // RS - Set motor speeds based on robot velocities. We use ROS coordinate convention: x-forward,
  // y-left, theta-CCW rotation.
  /* RS:robot_speed_x(m/s):robot_speed_y(m/s):robot_speed_theta(rad/s) */
  else if (cmd[0] == "RS")
  {
    float lin_speed_x = std::atof(cmd[1].c_str());
    float lin_speed_y = std::atof(cmd[2].c_str());
    float angular_speed_z = std::atof(cmd[3].c_str());

    float lin_speed_dir = atan2(lin_speed_y, lin_speed_x);
    float lin_speed_mag = sqrt(lin_speed_x * lin_speed_x + lin_speed_y * lin_speed_y);

    for (uint8_t i = 0; i < MOTOR_COUNT; i++)
    {
      float speed = lin_speed_mag * sin(lin_speed_dir - omnimotors.m[i].getWheelPosPhi()) +
                    omnimotors.m[i].getWheelPosR() * angular_speed_z;
      if (abs(speed) < 1e-5)
      {
        omnimotors.m[i].stop();
      }
      else
      {
        omnimotors.m[i].setSpeedSetPoint(speed);
      }
    }
    cmd_timer.reset();
  }
}

void OmniMotorsControl::stop() {
  for (uint8_t i = 0; i < MOTOR_COUNT; i++)
  {
    omnimotors.m[i].stop();
  }
}