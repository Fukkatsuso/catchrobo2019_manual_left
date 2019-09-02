/*
 * mode.h
 *
 *  Created on: 2019/08/26
 *      Author: mutsuro
 */

#ifndef MODE_H_
#define MODE_H_


class Mode
{
public:
	// 4bit
	enum State
	{
		Init 		= 0x0,
		Stay 		= 0x1,
		OwnArea 	= 0x2,
		CommonArea 	= 0x3,
		ShootingBox = 0x4
	};

	// 4bit
	enum Coordinate
	{
		Cartesian 	= 0x0,	// 直交座標系
		Polar 		= 0x1	// 極座標系
	};

	// 4bit
	enum Accelaration
	{
		LinearAcc 		= 0x0,
		NonLinearAcc	= 0x1
	};

	// 1bit
	enum Suction
	{
		Release = 0b0000,	// 通常
		Hold	= 0b0001	// 吸引
	};

	// 1bit
	enum Slider
	{
		Forward 	= 0b0000,
		Backward 	= 0b0010
	};

	static int generate(Mode::State state, Mode::Coordinate coord, Mode::Accelaration acc, Mode::Suction suction, Mode::Slider slider) {
		int ret = (state) | (coord << 4) | (acc << 8) | ((suction | slider) << 12);
		return ret;
	}

	static int state(int mode) {
		return ((mode) & 0xf);
	}

	static int coordinate(int mode) {
		return ((mode >> 4) & 0xf);
	}

	static int accelaration(int mode) {
		return ((mode >> 8) & 0xf);
	}

	static int suction(int mode) {
		return ((mode >> 12) & 0b0001);
	}

	static int slider(int mode) {
		return ((mode >> 12) & 0x0010);
	}
};


#endif /* MODE_H_ */
