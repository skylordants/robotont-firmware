#ifndef ODOM_H
#define ODOM_H

#include "mbed.h"
#include "Matrix.h"

#include "functional_modules/functional_module.h"
#include "hardware_modules/omnimotors.h"
#include "packetprocessor.h"

/** 
 * \brief Odometry class
 * This class calculates robot's position according to the wheel speed and their confifguration
 */
class Odom : public FunctionalModule
{
public:
  /**
   * \brief Constructor
   * \param packetprocessor Pointer to packetprocessor
   * \param omnimotors Pointer to omnimotors
   * \param delta_t period of how often the odometry will be updated
   */
  Odom(OmniMotors *omnimotors, float delta_t);
  ~Odom();

  void processPacket(const std::vector<std::string>& cmd);

  void loop();

  /**
   * \brief Resets the odometry
   * This function resets the odom frame velocity and position matrices to zero.
   */
  void reset();

  /**
   * \brief Updates the odometry
   * \param vel_1 Velocity of wheel 1
   * \param vel_2 Velocity of wheel 2
   * \param vel_3 Velocity of wheel 3
   *
   * This function calculates the new robot position based on its wheel speeds.
   */
  void update(float vel_1, float vel_2, float vel_3);

  /**
   * \todo FINISH DOCUMENTATION
   */
  float getPosX() const
  {
    return odom_pos_(1, 1);
  };

  float getPosY() const
  {
    return odom_pos_(2, 1);
  };

  float getOriZ() const
  {
    return odom_pos_(3, 1);
  };

  float getLinVelX() const
  {
    return robot_vel_(1, 1);
  };
  
  float getLinVelY() const
  {
    return robot_vel_(2, 1);
  };

  float getAngVelZ() const
  {
    return robot_vel_(3, 1);
  };

  /**
   * \brief Prints odom internals for debugging purposes
   */
  void print();


private:
  Matrix wheel_vel_; /**< Vector with wheel speeds [rad/s] */
  Matrix robot_vel_; /**< Velocity vector (dX, dY, dtheta) in robot frame */
  Matrix odom_vel_;  /**< Velocity vector (dx, dy, dtheta) in odom frame */
  Matrix odom_pos_;  /**< Position Vector (x, y, theta) in odom frame */

  Matrix odom_matrix_;     /**< Odometry matrix */
  Matrix odom_matrix_inv_; /**< Inverse of the odometry matrix */

  const float delta_t_;

  OmniMotors *omnimotors_;
};

#endif // ODOM_H
