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
struct Instruction
{
public:
	Instruction(){}

	Instruction(float x, float y, float z, float duration, int mode):
		x(x), y(y), z(z), duration(duration), mode(mode) {}

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

	float x;
	float y;
	float z;
	float duration;
	int mode;
};


#endif /* INSTRUCTION_H_ */
