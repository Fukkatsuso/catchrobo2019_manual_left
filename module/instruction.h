/*
 * instruction.h
 *
 *  Created on: 2019/08/26
 *      Author: mutsuro
 */

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "mbed.h"
#include "mode.h"


// モードと目標地点の設定
// 目標地点はフィールド基準で
class Instruction
{
public:
	void set(int mode, float duration, float x, float y, float z) {
		this->mode = mode;
		this->duration = duration;
		set_point(x, y, z);
	}

	void set_point(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	int get_mode() {
		return mode;
	}

	int get_mode_state() {
		return Mode::state(mode);
	}

	int get_mode_suction() {
		return Mode::suction(mode);
	}

	int get_mode_coord() {
		return Mode::coordinate(mode);
	}

	int get_mode_acc() {
		return Mode::accelaration(mode);
	}

	float get_duration() {
		return duration;
	}

	float get_x() {
		return x;
	}

	float get_y() {
		return y;
	}

	float get_z() {
		return z;
	}

private:
	int mode;
	float duration;
	float x;
	float y;
	float z;
};


#endif /* INSTRUCTION_H_ */
