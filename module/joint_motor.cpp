///*
// * joint_motor.cpp
// *
// *  Created on: 2019/06/27
// *      Author: mutsuro
// */
//
//#include "joint_motor.h"
//
//
//
//JointMotor::JointMotor(PwmOut* motor, Encoder* encoder):
//	motor(motor), encoder(encoder)
//{
//	motor->period_us(50);
//	motor->write(0.5);
//}
//
//
//void JointMotor::pid_setting(float arg_k[3], Timer* pid_timer)
//{
//	pid.set_gain(arg_k);
//	pid.set_timer(pid_timer);
//	pid.param_set_limit(0.4, -0.4);
//}
//
//
//void JointMotor::set_limit(float max, float min)
//{
//	dist_max = max;
//	dist_min = min;
//}
//
//
//float JointMotor::move_to(float target)
//{
////	target = limit(target, dist_max, dist_min);
//	float now = encoder->get_distance();
//	float duty = 0.5 + pid.calc(now, target);
//
//	motor->write(duty);
//
//	return duty;
//}
//
//
//float JointMotor::get_now()
//{
//	return encoder->get_distance();
//}
