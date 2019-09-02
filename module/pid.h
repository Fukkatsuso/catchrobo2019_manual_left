/*
 * pid.h
 *
 *  Created on: 2018/10/31
 *      Author: mutsuro
 *
 * ・ゲインを設定してインスタンス生成
 * ・
 */

#ifndef PID_H_
#define PID_H_

#include "mbed.h"


class PID
{
public:
	PID();
	PID(float arg_k[3]);
	PID(float arg_k[3], Timer* pid_timer);
	~PID();

	void set_timer(Timer* pid_timer);

	void set_gain(float arg_k[3]);
	void start(float obs_init=0, float opr_init=0);

	void param_update(float obs_now, float obs_tgt);
	void calc_P();
	void calc_I();
	void calc_D();
	float calc(float obs_now, float obs_tgt);

	void param_set_limit(float max, float min);
	void param_limit();

	float get_opr_nxt();

private:
	float k[3];

	Timer *timer;

	struct{
		float dif;
		float dif_min;
		float now;
	}time;

	struct{
		float nxt;
		float prv;
		float dif;

		float min;
		float max;
	}opr;//操作量

	struct{
		float dif;
		float dif_prv;
		float dif_prv_prv;

		float tgt;
	}obs;//センサー観測

	double P;
	double I;
	double D;
};


#endif /* PID_H_ */
