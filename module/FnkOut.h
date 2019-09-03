/*
 * FnkOut.h
 *
 *  Created on: 2018/09/03
 *      Author: S.Tsuboi
 */

#ifndef FNKOUT_H_
#define FNKOUT_H_

#include "mbed.h"

/******* warning ************************************************************
 *																			*
 *	このソースコードは作れといわれた深夜に眠気と戦いながら秒(1時間ぐらい)で							*
 *	書かれたものです。																*
 *	支離滅裂な思考・コメントアウトも多々見られると思います。										*
 *	デバッグも秒しかしていないので、致命的なバグを抱えている可能性も								*
 *	微粒子レベルよりちょい増しであるかと存じます。											*
 *	信用できないと判断した場合は、直ちに使用をやめ、											*
 *	お近くのFnk先輩にご相談下さい。													*
 *																			*
 *																	TBI		*
 ****************************************************************************/

class FnkOut{

public:

	//コンストラクタ
	//引数にPinName一つでいつものと一緒
	//というか普通のpwmを使ったほうが軽いやんって書いてから思った
	//
	//　　　PinName二つで回転方向とpwmでの制御
	//普段と同じように0.5で止まって0～1で正逆できるはず
	FnkOut(PinName _pwm, PinName _dir = NC);

	//0～1を突っ込む
	//0.5で止まる
	//0～0.5と0.5～1のどっちがcwでどっちがccwかはがんばって下さい
	//ブラシレスだと線入れ替えても回転方向変わらないので悪しからず。
	void write(float _value);

	//いつもの
	float read(void);

	//みたまんま
	void period(float _seconds){
		pwm->period(_seconds);
	}
	void period_ms(int _ms){
		pwm->period_ms(_ms);
	}
	void period_us(int _us){
		pwm->period_us(_us);
	}



#ifdef MBED_OPERATORS

	//いつもの
	FnkOut & operator=(float _value){
		write(_value);
		return *this;
	}
	//これ使ったことないけど一応
	FnkOut & operator=(FnkOut &_rhs){
		write(_rhs.read());
		return *this;
	}

	operator float(){
		return read();
	}

#endif

private:

	PwmOut *pwm;
	DigitalOut *dir;

	//制御方式
	//LAPがいつものやつ
	//SMBがpwmと回転方向つなぐやつ
	enum{
		LAP,
		SMB,
	} mode;

};


#endif /* FNKOUT_H_ */
