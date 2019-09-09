/*
 * fan.h
 *
 *  Created on: 2019/09/09
 *      Author: mutsuro
 */

#ifndef MODULE_FAN_H_
#define MODULE_FAN_H_


#include "mbed.h"
#include "functions.h"


template <class T>
class Fan
{
public:
	Fan(PinName pin) {
		pwm = new T(pin);
		pwm->period_ms(20);
		off();
	}

	// duty : 1020 ~ 2020
	int on(int max) {
		duty += limit(max - duty, 1, -1);
		duty = limit(duty, max, 1020);
		pwm->pulsewidth_us(duty);
		return duty;
	}

	int off() {
		duty = 1020;
		pwm->pulsewidth_us(duty);
		return duty;
	}

	int get_duty() {
		return duty;
	}

private:
	T* pwm;
	int duty;
};


#endif /* MODULE_FAN_H_ */
