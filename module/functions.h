/*
 * functions.h
 *
 *  Created on: 2019/06/24
 *      Author: mutsuro
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "mbed.h"


//limit
template <typename T>
T limit(T value, T max, T min);


inline float sqrt3(float a, float b, float c);


inline float sqrt2(float a, float b);


// sin加速をするときの速度の計算
float sin_accel(float period, float dist, float now_t);


float linear_accel(float period, float dist, float now_t);


int counter_update(int counter, float now, float target, float buff);


//cartesian -> polar
void cartesian2polar(float x, float y, float z, float* r, float* theta, float* phi);


void polar2cartesian(float r, float theta, float phi, float* x, float* y, float* z);


#endif /* FUNCTIONS_H_ */
