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


//extern float limit(float value, float max, float min);


// - example1
// PwmOut pwm(p21);
// Encoder enc(p13, p14);
// JointMotor<PwmOut> motor(&pwm, &enc, 1); // 正転duty:0.5~0.9
//
// - example2
// FnkOut pwm(p21, p22);
// Encoder enc(p13, p14);
// JointMotor<FnkOut> motor(&pwm, &enc, -1); // 正転duty:0.5~0.1

template <class T>
class JointMotor
{
public:
	JointMotor(T* motor, Encoder* encoder, int rot) :
		motor(motor), encoder(encoder), rot(rot) {
		motor->period_us(50);
		motor->write(0.5);
	}

	void pid_setting(float arg_k[3], Timer* pid_timer) {
		pid.set_gain(arg_k);
		pid.set_timer(pid_timer);
		pid.param_set_limit(0.4, -0.4);
	}

	void set_limit(float max, float min) {
		dist_max = max;
		dist_min = min;
	}

	float move_to(float target) {
//		target = limit(target, dist_max, dist_min);
		float now = encoder->get_distance();
		float duty = 0.5 + rot * pid.calc(now, target);
		motor->write(duty);

		return duty;
	}

	float get_now() {
		return encoder->get_distance();
	}

private:
	T *motor;
	Encoder *encoder;
	PID pid;

	const int rot;
	float dist_max;
	float dist_min;
};


#endif /* JOINT_MOTOR_H_ */
