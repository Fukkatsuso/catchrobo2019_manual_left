/*
 * joint_motor.h
 *
 *  Created on: 2019/06/27
 *      Author: mutsuro
 */

#ifndef JOINT_MOTOR_H_
#define JOINT_MOTOR_H_

#include "mbed.h"
#include "encoder.h"
#include "pid.h"


extern float limit(float value, float max, float min);

class JointMotor
{
public:
	JointMotor(PwmOut *motor, Encoder *encoder);
	void pid_setting(float arg_k[3], Timer* pid_timer);

	void set_limit(float max, float min);

	float move_to(float target);

	float get_now();

private:
	PwmOut *motor;
	Encoder *encoder;
	PID pid;

	float dist_max;
	float dist_min;
};


#endif /* JOINT_MOTOR_H_ */
