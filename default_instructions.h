/*
 * default_instructions.h
 *
 *  Created on: 2019/09/08
 *      Author: mutsuro
 */

#ifndef DEFAULT_INSTRUCTIONS_H_
#define DEFAULT_INSTRUCTIONS_H_

#include "module/polar_arm.h"
#include "module/instruction.h"


// (アームの取り付け位置のオフセット + 土台中心から駆動部基準点までのオフセット) : フィールド左端手前が基準
#define X_OFFSET 234.0f
#define Y_OFFSET -127.0f
#define Z_OFFSET 633.5f
// Φの回転半径
#define PHI_RADIUS 75.8f
// スライダーの移動距離
#define SLIDER_OFFSET 175.0f


Instruction neutral_inst(float duration, enum Mode::Coordinate coord, enum Mode::Suction suction)
{
	return Instruction(X_OFFSET, 250, INIT_Z-30, duration,
			Mode::Neutral, coord, Mode::NonLinearAcc, suction, Mode::Forward);
}

Instruction own_area_inst(float x, float y, float z, enum Mode::Accelaration acc)
{
	return Instruction(x, y, z, 2,
			Mode::OwnArea, Mode::Cartesian, acc, Mode::Release, Mode::Forward);
}

Instruction common_area_inst(float x, float y, float z, enum Mode::Accelaration acc)
{
	return Instruction(x, y, z, 2,
			Mode::CommonArea, Mode::Cartesian, acc, Mode::Release, Mode::Forward);
}

Instruction go_fast_one_inst(float duration, enum Mode::Accelaration acc)
{
	return Instruction(X_OFFSET, 230, 170, duration,
			Mode::OwnArea, Mode::Cartesian, acc, Mode::Release, Mode::Forward);
}

Instruction above_fast_one_box_inst(float duration, enum Mode::Coordinate coord, enum Mode::Suction suction)
{
	return Instruction(375, -170, 650, duration,
			Mode::ShootingBox, coord, Mode::NonLinearAcc, suction, Mode::Backward);
}

Instruction above_box_front_inst(float duration, enum Mode::Coordinate coord, enum Mode::Suction suction)
{
	return Instruction(540, -145, 700, duration,
			Mode::ShootingBox, coord, Mode::NonLinearAcc, suction, Mode::Forward);
}

Instruction above_box_rear_inst(float duration, enum Mode::Coordinate coord, enum Mode::Suction suction)
{
	return Instruction(540, -350, 700, duration,
			Mode::ShootingBox, coord, Mode::NonLinearAcc, suction, Mode::Backward);
}

Instruction stay_inst(float x, float y, float z, float duration, enum Mode::Suction suction, enum Mode::Slider slider)
{
	return Instruction(x, y, z, duration,
			Mode::Stay, Mode::Cartesian, Mode::LinearAcc, suction, slider);
}

// x, y, z, duration,
//   state, coord, acc, suction, slider
#define INST_NUM 1
Instruction default_inst[INST_NUM] =
{
		// 初期化
		Instruction(INIT_X,	INIT_Y,	INIT_Z,	1,
				Mode::Init,	Mode::Polar,	Mode::NonLinearAcc, Mode::Release, Mode::Backward)
};


#endif /* DEFAULT_INSTRUCTIONS_H_ */
