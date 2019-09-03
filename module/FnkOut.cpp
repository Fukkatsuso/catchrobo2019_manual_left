/*
 * FnkOut.cpp
 *
 *  Created on: 2018/09/03
 *      Author: S.Tsuboi
 */

#include "FnkOut.h"

FnkOut::FnkOut(PinName _pwm, PinName _dir){

	if(_dir != NC)mode = SMB;
	else mode = LAP;

	pwm = new PwmOut(_pwm);
	dir = new DigitalOut(_dir);

}


void FnkOut::write(float _value){

	if(_value > 1)_value = 1;
	else if(_value < 0)_value = 0;

	if(mode == SMB){
		_value -= 0.5;
		_value *= 2;

		if(_value > 0)dir->write(1);
		else dir->write(0);

		_value = fabsf(_value);

		pwm->write(_value);
	}
	else pwm->write(_value);

}

float FnkOut::read(void){

	float __value = pwm->read();

	if(mode == SMB){
		__value /= 2.0;

		if(!dir->read())__value *= - 1.0;

		__value += 0.5;

	}

	return __value;

}
