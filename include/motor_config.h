#ifndef MOTOR_CONFIG_TEMPLATE
#define MOTOR_CONFIG_TEMPLATE

#include "mbed.h"

// Common parameters for all motors
#define ENC_CPR 64
#define GEAR_RATIO 18.75
#define WHEEL_RADIUS 0.035
#define WHEEL_POS_R 0.145
#define PID_KP 0.8
#define PID_TI 0.05
#define PID_TD 0.0
#define PID_DELTA_T 0.01
#define MAIN_DELTA_T 0.02

/**
 * @brief A configuration for the Motor
 */
struct MotorConfig
{
  PinName pin_dir1; 
  PinName pin_dir2;
  PinName pin_pwm;
  PinName pin_enca;
  PinName pin_encb;
  PinName pin_fault;
  PinName pin_feedback;

  //Default PID parameters
  float pid_k_p;
  float pid_tau_i;
  float pid_tau_d;
  float pid_dt;

  float enc_cpr; // encoder counts per revolution
  float gear_ratio; // gearbox reduction ratio
  float wheel_radius; // wheel outer radius

  // wheel position in polar coordinates
  float wheel_pos_r; // distance from center
  float wheel_pos_phi; // angle relative to x-axis (forward)
};

extern struct MotorConfig cfg0;
extern struct MotorConfig cfg1;
extern struct MotorConfig cfg2;

#endif