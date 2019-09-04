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
	HandServo(PinName pin, float rad2pwm, float offset_rad) : rad2pwm(rad2pwm), offset_rad(offset_rad) {
		servo = new T_pwm(pin);
		servo->period_ms(20);
	}

	int keep(float rad) {
		rad -= offset_rad;
		int duty = limit((int)(rad * rad2pwm), 2500, 500);
		servo->pulsewidth_us(duty);
		return duty;
	}

private:
	T_pwm* servo;
	const float rad2pwm;
	const float offset_rad;
};


#endif /* MODULE_HAND_SERVO_H_ */
