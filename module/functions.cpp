/*
 * functions.cpp
 *
 *  Created on: 2019/06/24
 *      Author: mutsuro
 */

#include "functions.h"


int limit(int value, int max, int min)
{
	if (value > max) return max;
	else if (value < min) return min;
	return value;
}


float limit(float value, float max, float min)
{
	if (value > max) return max;
	else if (value < min) return min;
	return value;
}


inline float sqrt3(float a, float b, float c)
{
	return sqrt(a*a + b*b + c*c);
}


inline float sqrt2(float a, float b)
{
	return sqrt(a*a + b*b);
}


float sin_accel_vel(float period, float dist, float now_t)
{
	if (now_t > period) return 0;

	float cosine = cos(2.0 * M_PI * now_t / period);
	return dist * (1.0 - cosine) / period;
}


float linear_accel_vel(float period, float dist, float now_t)
{
	if (now_t > period) return 0;

	return dist / period;
}


float sin_accel_pos(float period, float dist, float now_t)
{
	if (now_t > period) return dist;

	float sine = sin(2.0 * M_PI * now_t / period);
	return (dist / period) * (now_t - (period / (2.0 * M_PI)) * sine);
}


float linear_accel_pos(float period, float dist, float now_t)
{
	if (now_t > period) return dist;

	return dist * now_t / period;
}


int counter_update(int counter, float now, float target, float buff)
{
	bool ok = (fabs(target - now) < buff);
	counter += (ok ? 1 : -1);
	return (counter >= 0 ? counter : 0);
}


//定義域要再考
void cartesian2polar(float x, float y, float z, float phi_radius, float* r, float* theta, float* phi)
{
	if (x == 0) *theta = M_PI / 2.0;
	else if (y == 0 && x > 0) *theta = 0;
	else if (y == 0 && x < 0) *theta = M_PI;
	else *theta = atan2(y, x);

	// phiに回転半径が存在するので極座標とは異なる計算法になる
	double R = sqrt2(x, y);
	double D = sqrt3(x, y, z);
	*phi = acos(- phi_radius / D) - atan2(z, R);
	*r = (R + phi_radius * cos(*phi)) / sin(*phi);
}


void polar2cartesian(float r, float theta, float phi, float phi_radius, float* x, float* y, float* z)
{
	float R = r * sin(phi) - phi_radius * cos(phi);
	*x = R * cos(theta);
	*y = R * sin(theta);
	*z = (r * cos(phi) + phi_radius * sin(phi));
}
