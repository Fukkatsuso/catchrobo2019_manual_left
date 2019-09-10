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
#define INIT_X 346.0f
#define INIT_Y -127.0f
#define INIT_Z 633.5+75.8

/*
 * input  : 目標座標
 * output : 各軸の毎時の目標回転角
 */
class PolarArm
{
public:
	PolarArm(float offset_x, float offset_y, float offset_z, float phi_radius, float offset_slider);
	void restart(float target_x, float target_y, float target_z, enum Mode::Slider slider);
	void update_target(float target_x, float target_y, float target_z, enum Mode::Slider slider);
	void set_duration(float duration);
	void set_mode(enum Mode::Coordinate coord, enum Mode::Accelaration acc, enum Mode::Slider slider);

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
	float phi_radius;
	const float slider_offset;
};


#endif /* POLAR_ARM_H_ */
