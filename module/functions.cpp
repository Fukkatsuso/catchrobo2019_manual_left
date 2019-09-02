/*
 * functions.cpp
 *
 *  Created on: 2019/06/24
 *      Author: mutsuro
 */

#include "functions.h"


template <typename T>
T limit(T value, T max, T min)
{
	if (value > max) return max;
	else if (value < min) return min;
	return value;
}

template int limit<int> (int value, int max, int min);
template float limit<float> (float value, float max, float min);
template double limit<double> (double value, double max, double min);


inline float sqrt3(float a, float b, float c)
{
	return sqrt(a*a + b*b + c*c);
}


inline float sqrt2(float a, float b)
{
	return sqrt(a*a + b*b);
}


float sin_accel(float period, float dist, float now_t)
{
	if (now_t > period) return 0;

	float cosine = cos(2.0 * M_PI * now_t / period);
	return dist * (1.0 - cosine) / period;
}


float linear_accel(float period, float dist, float now_t)
{
	if (now_t > period) return 0;
	return dist / period;
}


int counter_update(int counter, float now, float target, float buff)
{
	bool ok = (fabs(target - now) < buff);
	counter += (ok ? 1 : -1);
	return (counter >= 0 ? counter : 0);
}


//定義域要再考
void cartesian2polar(float x, float y, float z, float* r, float* theta, float* phi)
{
	*r = sqrt3(x, y, z);
	*theta = acos(z / (*r)); // 0~PI
	if (x == 0 && y == 0);
	else *phi = acos(x / sqrt2(x, y));  //default:-PI~PI
}


void polar2cartesian(float r, float theta, float phi, float* x, float* y, float* z)
{
	*x = r * sin(theta) * cos(phi);
	*y = r * sin(theta) * sin(phi);
	*z = r * cos(theta);
}
