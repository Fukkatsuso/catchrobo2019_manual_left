/*
 * functions.h
 *
 *  Created on: 2019/06/24
 *      Author: mutsuro
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "mbed.h"


int limit(int value, int max, int min);
float limit(float value, float max, float min);


inline float sqrt3(float a, float b, float c);


inline float sqrt2(float a, float b);


// sin加速をするときの速度の計算
float sin_accel_vel(float period, float dist, float now_t);


float linear_accel_vel(float period, float dist, float now_t);


float sin_accel_pos(float period, float dist, float now_t);


float linear_accel_pos(float period, float dist, float now_t);


int counter_update(int counter, float now, float target, float buff);


//cartesian -> polar
void cartesian2polar(float x, float y, float z, float phi_radius, float* r, float* theta, float* phi);


void polar2cartesian(float r, float theta, float phi, float phi_radius, float* x, float* y, float* z);


#endif /* FUNCTIONS_H_ */
