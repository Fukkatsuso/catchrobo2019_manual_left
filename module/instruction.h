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

	Instruction(float x, float y, float z, float duration,
			enum Mode::State state,
			enum Mode::Coordinate coord,
			enum Mode::Accelaration acc,
			enum Mode::Suction suction,
			enum Mode::Slider slider):
		x(x), y(y), z(z), duration(duration),
		state(state), coord(coord), acc(acc), suction(suction), slider(slider)
		{}

	float x;
	float y;
	float z;
	float duration;
	Mode::State state;
	Mode::Coordinate coord;
	Mode::Accelaration acc;
	Mode::Suction suction;
	Mode::Slider slider;
};


#endif /* INSTRUCTION_H_ */
