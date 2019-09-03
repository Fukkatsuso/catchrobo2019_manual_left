#include "mbed.h"
#include "init2.h"
#include "pin_assign.h"
#include "module/polar_arm.h"
#include "module/joint_motor.h"
#include "module/mode.h"
#include "module/instruction.h"
#include "module/queue.h"


// (アームの取り付け位置のオフセット + 土台中心から駆動部基準点までのオフセット) : フィールド左端手前が基準
#define X_OFFSET (0 + 0)
#define Y_OFFSET (0 + 0)
#define Z_OFFSET (0 + 0)
// スライダーの移動距離
#define SLIDER_OFFSET 175
PolarArm catcher(X_OFFSET, Y_OFFSET, Z_OFFSET, SLIDER_OFFSET);

JointMotor motor_r(&pwm_r, &enc_r);
JointMotor motor_theta(&pwm_theta, &enc_theta);
JointMotor motor_phi(&pwm_phi, &enc_phi);

Timer pid_timer;


void led_all(int onoff)
{
	led1.write(onoff); led2.write(onoff); led3.write(onoff); led4.write(onoff);
}


#define BUFF_ARRIVE 3 // [mm]
#define WAIT_ARRIVE 10
inline bool has_arrived(int cnt_x, int cnt_y, int cnt_z)
{
	return ((cnt_x >= WAIT_ARRIVE) && (cnt_y >= WAIT_ARRIVE) && (cnt_z >= WAIT_ARRIVE));
}


int main(){
	led_all(1);

	float pid_gain_r[3] 	= {0, 0, 0};
	float pid_gain_theta[3] = {0, 0, 0};
	float pid_gain_phi[3] 	= {0, 0, 0};

	int x_cnt_arrive = 0;
	int y_cnt_arrive = 0;
	int z_cnt_arrive = 0;

	int mode;
	float duration;
	Instruction inst;
	Queue<Instruction> queue_inst;
	Queue<Instruction> queue_buff;

	float r_next;
	float theta_next;
	float phi_next;

	float x_now;
	float y_now;
	float z_now;

	// pspadの入力で微調整する量
	float x_adjust;
	float y_adjust;
	float z_adjust;

	// 指令をqueueに入れ込む
	mode = Mode::generate(Mode::Init, Mode::Polar, Mode::NonLinearAcc, Mode::Release, Mode::Backward);
	duration = 1.5;
	inst.set(mode, duration, INIT_X, INIT_Y, INIT_Z);
	queue_inst.push(inst);

	wait_ms(300);
	servo.period_ms(20);
	pc.baud(921600);

	motor_r.pid_setting(pid_gain_r, &pid_timer);
	motor_theta.pid_setting(pid_gain_theta, &pid_timer);
	motor_phi.pid_setting(pid_gain_phi, &pid_timer);

	inst = queue_inst.front();
	mode = inst.get_mode();
	catcher.restart(inst.get_x(), inst.get_y(), inst.get_z());
	catcher.set_duration(inst.get_duration());
	catcher.set_mode(inst.get_mode());

	slider.write(0);
	led_all(0);

	while(1){
		AdjustCycle(1000);

		// 指令に従って次の位置を計算
		catcher.calc_next();

		r_next = catcher.get_r_next();
		theta_next = catcher.get_theta_next();
		phi_next = catcher.get_phi_next();

		// 次の位置に移動
		motor_r.move_to(r_next);
		motor_theta.move_to(theta_next);
		motor_phi.move_to(phi_next);

		slider.write(Mode::slider(mode) >> 1);

		polar2cartesian(motor_r.get_now(), motor_theta.get_now(), motor_phi.get_now(),
				&x_now, &y_now, &z_now);
		x_now += X_OFFSET;
		y_now += Y_OFFSET;
		z_now += Z_OFFSET;

		x_cnt_arrive = counter_update(x_cnt_arrive, x_now, inst.get_x(), BUFF_ARRIVE);
		y_cnt_arrive = counter_update(y_cnt_arrive, y_now, inst.get_y(), BUFF_ARRIVE);
		z_cnt_arrive = counter_update(z_cnt_arrive, z_now, inst.get_z(), BUFF_ARRIVE);

		// リトライに備えて実行済みinstをバッファに保存
		// 初期位置に戻ったらqueue_buffをクリア
		if (has_arrived(x_cnt_arrive, y_cnt_arrive, z_cnt_arrive)) {
			if (inst.get_mode_state() == Mode::Init) {
				queue_buff.clear();
			}
			else {
				queue_buff.push(inst);
			}
			if (queue_inst.length() > 0) {
				queue_inst.pop();
				x_adjust = 0;
				y_adjust = 0;
				z_adjust = 0;
			}

			inst = queue_inst.front();
			catcher.restart(inst.get_x(), inst.get_y(), inst.get_z());
			catcher.set_duration(inst.get_duration());
			catcher.set_mode(inst.get_mode());
		}

		if (pc.readable()) {
			pc.printf("x: %4.0f  y: %4.0f  z: %4.0f  ", x_now, y_now, z_now);
		}
	}

	return 0;
}

