/*
 * polar_arm.cpp
 *
 *  Created on: 2019/08/28
 *      Author: mutsuro
 */

#include "polar_arm.h"


PolarArm::PolarArm(float offset_x, float offset_y, float offset_z, float offset_slider) :
	x_offset(offset_x), y_offset(offset_y), z_offset(offset_z), slider_offset(offset_slider)
{
	x.target = INIT_X - x_offset;
	y.target = INIT_Y - y_offset;
	z.target = INIT_Z - z_offset;
	mode_slider = Mode::Backward;
	update_target(INIT_X, INIT_Y, INIT_Z);
}


// 座標はフィールド基準
bool PolarArm::set_target(float target_x, float target_y, float target_z,
		float duration, int mode)
{
	this->duration = duration;

	mode_coord = (Mode::Coordinate)Mode::coordinate(mode);
	mode_acc = (Mode::Accelaration)Mode::accelaration(mode);
	mode_slider = (Mode::Slider)Mode::slider(mode);

	update_target(target_x, target_y, target_z);

	clock_prev = 0;
	clock.reset();
	clock.start();

	return true;
}


void PolarArm::calc_next()
{
	float clock_now = clock.read();
	float dt = clock_now - clock_prev;
	clock_prev = clock_now;

	switch (mode_coord) {
	case Mode::Cartesian:
		calc_vel_cartesian(clock_now);
		x.next = x.now + x.vel * dt;
		y.next = y.now + y.vel * dt;
		z.next = z.now + z.vel * dt;
		cartesian2polar(x.next, y.next, z.next, &r.next, &theta.next, &phi.next);
		x.now = x.next;
		y.now = y.next;
		z.now = z.next;
		break;
	case Mode::Polar:
		calc_vel_polar(clock_now);
		r.next	 	= r.now 	+ r.vel * dt;
		theta.next 	= theta.now + theta.vel * dt;
		phi.next 	= phi.now 	+ phi.vel * dt;
		break;
	}

	r.now = r.next;
	theta.now = theta.next;
	phi.now = phi.next;
}


float PolarArm::get_r_next()
{
	return r.next;
}


float PolarArm::get_theta_next()
{
	return theta.next;
}


float PolarArm::get_phi_next()
{
	return phi.next;
}


void PolarArm::update_target(float target_x, float target_y, float target_z)
{
	// 初期位置の更新
	x.start = x.target;
	y.start = y.target;
	z.start = z.target;
	x.now 	= x.start;
	y.now 	= y.start;
	z.now 	= z.start;
	// 目標位置の更新
	// offset処理も忘れずに
	if (mode_slider == Mode::Forward) target_y -= slider_offset;
	x.target = target_x - x_offset;
	y.target = target_y - y_offset;
	z.target = target_z - z_offset;
	//
	x.dist = x.target - x.start;
	y.dist = y.target - y.start;
	z.dist = z.target - z.start;

	// r, theta, phi に変換
	cartesian2polar(x.start, y.start, z.start, &r.start, &theta.start, &phi.start);
	cartesian2polar(x.target, y.target, z.target, &r.target, &theta.target, &phi.target);
	r.now 		= r.start;
	theta.now 	= theta.start;
	phi.now 	= phi.start;
	r.dist 		= r.target - r.start;
	theta.dist 	= theta.target - theta.start;
	phi.dist 	= phi.target - phi.start;
}


void PolarArm::calc_vel_cartesian(float now_t)
{
	switch (mode_acc) {
	case Mode::LinearAcc:
		x.vel = linear_accel(duration, x.dist, now_t);
		y.vel = linear_accel(duration, y.dist, now_t);
		z.vel = linear_accel(duration, z.dist, now_t);
		break;
	case Mode::NonLinearAcc:
		x.vel = sin_accel(duration, x.dist, now_t);
		y.vel = sin_accel(duration, y.dist, now_t);
		z.vel = sin_accel(duration, z.dist, now_t);
		break;
	}
}


void PolarArm::calc_vel_polar(float now_t)
{
	switch (mode_acc) {
	case Mode::LinearAcc:
		r.vel 		= linear_accel(duration, r.dist, now_t);
		theta.vel 	= linear_accel(duration, theta.dist, now_t);
		phi.vel 	= linear_accel(duration, phi.dist, now_t);
		break;
	case Mode::NonLinearAcc:
		r.vel 		= sin_accel(duration, r.dist, now_t);
		theta.vel 	= sin_accel(duration, theta.dist, now_t);
		phi.vel 	= sin_accel(duration, phi.dist, now_t);
		break;
	}
}
