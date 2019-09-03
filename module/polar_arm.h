/*
 * polar_arm.h
 *
 *  Created on: 2019/06/27
 *      Author: mutsuro
 */

#ifndef POLAR_ARM_H_
#define POLAR_ARM_H_

#include "mbed.h"
#include "mode.h"
#include "functions.h"


// x,y,zの初期値
// フィールド基準
#define INIT_X 100
#define INIT_Y 100
#define INIT_Z 100

/*
 * input  : 目標座標
 * output : 各軸の毎時の目標回転角
 */
class PolarArm
{
public:
	PolarArm(float offset_x, float offset_y, float offset_z, float offset_slider);
	void restart(float target_x, float target_y, float target_z);
	void set_target(float target_x, float target_y, float target_z);
	void set_duration(float duration);
	void set_mode(int mode);

	void calc_next();

	float get_r_next();
	float get_theta_next();
	float get_phi_next();

protected:
	void calc_pos_cartesian(float now_t);
	void calc_pos_polar(float now_t);

private:
	Timer clock;

	struct {
		float next;
		float start;
		float target;
		float dist;
	} x, y, z,
		r, theta, phi;

	float duration;
	Mode::Coordinate mode_coord;
	Mode::Accelaration mode_acc;
	Mode::Slider mode_slider;

	float x_offset;
	float y_offset;
	float z_offset;
	const float slider_offset;
};


#endif /* POLAR_ARM_H_ */
