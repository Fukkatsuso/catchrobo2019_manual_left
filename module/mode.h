/*
 * mode.h
 *
 *  Created on: 2019/08/26
 *      Author: mutsuro
 */

#ifndef MODE_H_
#define MODE_H_


namespace Mode
{
	enum State
	{
		Init,
		Stay,
		OwnArea,
		CommonArea,
		ShootingBox
	};

	enum Coordinate
	{
		Cartesian,	// 直交座標系
		Polar		// 極座標系
	};

	enum Accelaration
	{
		LinearAcc,
		NonLinearAcc
	};

	enum Suction
	{
		Release,	// 通常
		Hold	 	// 吸引
	};

	enum Slider
	{
		Backward 	= 0,
		Forward		= 1
	};
}


#endif /* MODE_H_ */
