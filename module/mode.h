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
		Init, // 初期位置
		Stay, // 停止待機
		Neutral, // コマンド待ち場所, 中継地点
		OwnArea, // 自陣
		CommonArea, // 共通エリア
		ShootingBox // シューティングbox
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
