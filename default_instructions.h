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


// x, y, z, duration,
//   state, coord, acc, suction, slider
#define INST_NUM 11
Instruction default_inst[INST_NUM] =
{
		// 初期化
		Instruction(INIT_X,	INIT_Y,	INIT_Z,	1,
				Mode::Init,	Mode::Polar,	Mode::NonLinearAcc, Mode::Release, Mode::Backward),
		// 開始待ち
		Instruction(INIT_X,	INIT_Y,	INIT_Z,	8,
				Mode::Stay,	Mode::Polar,	Mode::NonLinearAcc, Mode::Release, Mode::Backward),
		// go
		Instruction(220,	50,	INIT_Z,	1,
				Mode::Neutral,	Mode::Polar,	Mode::NonLinearAcc, Mode::Release, Mode::Backward),
		Instruction(220,	120,	170,	2,
				Mode::OwnArea,	Mode::Cartesian,	Mode::LinearAcc, Mode::Release, Mode::Backward),
		Instruction(220, 	120,	170,	1.5,
				Mode::Stay,	Mode::Cartesian,	Mode::LinearAcc,	Mode::Hold,	Mode::Backward),
		// return
		Instruction(220,	50,	INIT_Z,	2,
				Mode::Neutral,	Mode::Cartesian,	Mode::LinearAcc, Mode::Hold, Mode::Backward),
		Instruction(504,	-160,	INIT_Z,	1,
				Mode::ShootingBox,	Mode::Polar,	Mode::NonLinearAcc, Mode::Hold, Mode::Backward),
		Instruction(485,	-154,	500,	1,
				Mode::ShootingBox,	Mode::Polar,	Mode::NonLinearAcc, Mode::Hold, Mode::Backward),
		Instruction(485,	-154,	500,	0.5,
				Mode::Stay,	Mode::Polar,	Mode::NonLinearAcc, Mode::Release, Mode::Backward),
		Instruction(504,	-160,	INIT_Z,	1,
				Mode::Init,	Mode::Polar,	Mode::NonLinearAcc, Mode::Release, Mode::Backward),

		Instruction(220,	0,	INIT_Z,	1,
				Mode::Neutral,	Mode::Polar,	Mode::NonLinearAcc, Mode::Release, Mode::Backward),
};


#endif /* DEFAULT_INSTRUCTIONS_H_ */
