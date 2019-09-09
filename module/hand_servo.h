/*
 * hand_servo.h
 *
 *  Created on: 2019/09/04
 *      Author: mutsuro
 */

#ifndef MODULE_HAND_SERVO_H_
#define MODULE_HAND_SERVO_H_

#include "mbed.h"


extern int limit(int value, int max, int min);


template <class T_pwm>
class HandServo
{
public:
	HandServo(PinName pin, float min_rad, float max_rad, int min_duty, int max_duty, float offset_rad) :
		min_rad(min_rad), max_rad(max_rad), min_duty(min_duty), max_duty(max_duty), offset_rad(offset_rad) {
		servo = new T_pwm(pin);
		servo->period_ms(20);
	}

	int keep(float rad) {
		int duty = min_duty + (max_duty - min_duty) * (rad - offset_rad) / (max_rad - min_rad);
		duty = limit(duty, 2500, 500);
		servo->pulsewidth_us(duty);
		return duty;
	}

private:
	T_pwm* servo;
	float min_rad;
	float max_rad;
	int min_duty;
	int max_duty;
	const float offset_rad;
};


#endif /* MODULE_HAND_SERVO_H_ */
