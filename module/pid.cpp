/*
 * pid.cpp
 *
 *  Created on: 2018/10/31
 *      Author: mutsuro
 */

#include "pid.h"


PID::PID()
{
	time.dif_min = 0.0001;
}


PID::PID(float arg_k[3])
{
	set_gain(arg_k);
    time.dif_min = 0.0001;
}


PID::PID(float arg_k[3], Timer* pid_timer)
{
	set_gain(arg_k);
    time.dif_min = 0.0001;

    set_timer(pid_timer);
}


PID::~PID(){}


void PID::set_timer(Timer* pid_timer)
{
	timer = pid_timer;
	start();
}


void PID::set_gain(float arg_k[3])
{
	for (int i=0; i<3; i++)
		k[i] = arg_k[i];
}


//必要最低限の初期化
void PID::start(float obs_init, float opr_init)
{
	//積分値ゼロ初期化
	I = 0;

	//最初は停止しているとする
	obs.dif = 0;
	obs.dif_prv = 0;

	//後でコレが_opr.prvの値になる
	opr.nxt = opr_init;

	timer->reset();
	timer->start();

	time.now = 0;
}


void PID::param_update(float obs_now, float obs_tgt)
{
	opr.prv = opr.nxt;

	obs.tgt = obs_tgt;

	//観測値と目標値の偏差
	obs.dif_prv_prv = obs.dif_prv;
	obs.dif_prv = obs.dif;
	obs.dif = obs.tgt - obs_now;

	//ループ時間
	float time_prv = time.now;
	time.now = timer->read();
	time.dif = time.now - time_prv;
}


float PID::calc(float obs_now, float obs_tgt)
{
	param_update(obs_now, obs_tgt);

	calc_P();
	calc_I();
	calc_D();

	opr.dif = k[0]*P + k[1]*I + k[2]*D;
	opr.nxt = opr.prv + opr.dif;
	param_limit();

	return opr.nxt;
}


void PID::calc_P()
{
	P = obs.dif - obs.dif_prv;
}


void PID::calc_I()
{
	I += obs.dif * time.dif;
}


void PID::calc_D()
{
	D = (obs.dif - 2.0 * obs.dif_prv + obs.dif_prv_prv)
			/ time.dif;
}


void PID::param_set_limit(float max, float min)
{
	opr.max = max;
	opr.min = min;
}


void PID::param_limit()
{
	if(opr.nxt > opr.max)opr.nxt = opr.max;
	else if(opr.nxt < opr.min)opr.nxt = opr.min;
}


float PID::get_opr_nxt()
{
	return opr.nxt;
}
