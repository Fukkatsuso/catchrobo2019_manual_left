#include "mbed.h"
#include "init2.h"
#include "pin_assign.h"
#include "module/polar_arm.h"
#include "module/joint_motor.h"
#include "module/mode.h"
#include "module/instruction.h"
#include "module/queue.h"
#include "default_instructions.h"


PolarArm catcher(X_OFFSET, Y_OFFSET, Z_OFFSET, PHI_RADIUS, SLIDER_OFFSET);

JointMotor<FnkOut> motor_r(&pwm_r, &enc_r, 1);
JointMotor<PwmOut> motor_theta(&pwm_theta, &enc_theta, 1);
JointMotor<PwmOut> motor_phi(&pwm_phi, &enc_phi, -1);

Timer pid_timer;
Timer timer;


void led_all(int onoff)
{
	led1.write(onoff); led2.write(onoff); led3.write(onoff); led4.write(onoff);
}


#define BUFF_ARRIVE_X 15 // [mm]
#define BUFF_ARRIVE_Y 10
#define BUFF_ARRIVE_Z 15
#define WAIT_ARRIVE 80
inline bool has_arrived(int cnt_x, int cnt_y, int cnt_z)
{
	return ((cnt_x >= WAIT_ARRIVE) && (cnt_y >= WAIT_ARRIVE) && (cnt_z >= WAIT_ARRIVE));
}

struct Monitor_cartesian
{
public:
	Monitor_cartesian() : cnt_arrive(0), pos_adjust(0) {}
	int cnt_arrive;
	float pos_now;
	float pos_adjust;
};

struct Monitor_polar
{
public:
	Monitor_polar(float kp, float ki, float kd) {
		pid_gain[0] = kp;
		pid_gain[1] = ki;
		pid_gain[2] = kd;
		duty = 0.5;
	}
	float pid_gain[3];
	float pos_now;
	float pos_next;
	float duty;
};


int main(){
	Monitor_cartesian x;
	Monitor_cartesian y;
	Monitor_cartesian z;

	Monitor_polar r(0.009, 0, 0.00001);
	Monitor_polar theta(0.9, 0, 0.06);
	Monitor_polar phi(0.6, 0, 0.01);

	float now_t;

	Queue<Instruction> queue_inst;
	Queue<Instruction> queue_buff;

	// 指令をqueueに入れ込む
	Instruction inst;
	for (int i=0; i<INST_NUM; i++) {
		queue_inst.push(default_inst[i]);
	}

	led_all(1);

	wait_ms(300);
	pc.baud(921600);

	motor_r.pid_setting(r.pid_gain, &pid_timer);
	motor_theta.pid_setting(theta.pid_gain, &pid_timer);
	motor_phi.pid_setting(phi.pid_gain, &pid_timer);

	inst = queue_inst.front();
	catcher.restart(inst.x, inst.y, inst.z);
	catcher.set_duration(inst.duration);
	catcher.set_mode(inst.coord, inst.acc, inst.slider);

	slider.write(0);
	led_all(0);

	while(1){
		AdjustCycle(1000);

		now_t = timer.read();

		// 指令に従って次の位置を計算
		catcher.calc_next();

		r.pos_next = limit(catcher.get_r_next(), 651.0, R_OFFSET);
		theta.pos_next = catcher.get_theta_next();
		phi.pos_next = limit(catcher.get_phi_next(), M_PI, M_PI/2.0);

		// 次の位置に移動
		r.duty = motor_r.move_to(r.pos_next);
		theta.duty = motor_theta.move_to(theta.pos_next);
		phi.duty = motor_phi.move_to(phi.pos_next);
		// y方向スライド
		slider.write(inst.slider);
		// ハンドのサーボ
		servo.keep(phi.pos_next);

		// 現在値取得
		r.pos_now = motor_r.get_now();
		theta.pos_now = motor_theta.get_now();
		phi.pos_now = motor_phi.get_now();
		polar2cartesian(r.pos_now, theta.pos_now, phi.pos_now, PHI_RADIUS,
				&(x.pos_now), &(y.pos_now), &(z.pos_now));
		x.pos_now += X_OFFSET;
		y.pos_now += Y_OFFSET;
		z.pos_now += Z_OFFSET;

		x.cnt_arrive = counter_update(x.cnt_arrive, x.pos_now, inst.x, BUFF_ARRIVE_X);
		y.cnt_arrive = counter_update(y.cnt_arrive, y.pos_now, inst.y, BUFF_ARRIVE_Y);
		z.cnt_arrive = counter_update(z.cnt_arrive, z.pos_now, inst.z, BUFF_ARRIVE_Z);
		if (inst.state == Mode::Stay) {
			if (now_t < inst.duration) {
				x.cnt_arrive = 0;
				y.cnt_arrive = 0;
				z.cnt_arrive = 0;
			}
		}

		if (has_arrived(x.cnt_arrive, y.cnt_arrive, z.cnt_arrive)) {
			// リトライに備えて実行済みinstをバッファに保存
			// 初期位置に戻ったらqueue_buffをクリア
			if (inst.state == Mode::Init) {
				queue_buff.clear();
			}
			else {
				queue_buff.push(inst);
			}

			if (queue_inst.length() > 1) {
				queue_inst.pop();
				x.pos_adjust = 0;
				y.pos_adjust = 0;
				z.pos_adjust = 0;
				x.cnt_arrive = 0;
				y.cnt_arrive = 0;
				z.cnt_arrive = 0;
				inst = queue_inst.front();
				catcher.set_duration(inst.duration);
				catcher.set_mode(inst.coord, inst.acc, inst.slider);
				catcher.restart(inst.x, inst.y, inst.z);
				timer.reset();
				timer.start();
			}
		}

		pc.printf("now: %2.2f  ", now_t);
		pc.printf("x: %4.1f->%4.1f   y: %4.1f->%4.1f  z: %4.1f->%4.1f  ", x.pos_now, inst.x, y.pos_now, inst.y, z.pos_now, inst.z);
		pc.printf("r: %4.1f->%4.1f  theta: %3.1f->%3.1f  phi: %3.1f->%3.1f  ",
				r.pos_now, r.pos_next,
				rad2degree(theta.pos_now), rad2degree(theta.pos_next),
				rad2degree(phi.pos_now), rad2degree(phi.pos_next));
		pc.printf("duty: %1.4f %1.4f %1.4f  ", r.duty, theta.duty, phi.duty);
		pc.printf("\r\n");
	}

	return 0;
}

