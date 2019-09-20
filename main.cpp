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
Timer timer_shooting;


void led_all(int onoff)
{
	led1.write(onoff); led2.write(onoff); led3.write(onoff); led4.write(onoff);
}

#define X_ADJUST (x.pos_adjust/2)
#define Y_ADJUST (y.pos_adjust/2)
#define Z_ADJUST (z.pos_adjust/2)

#define BUFF_ARRIVE_X 15 // [mm]
#define BUFF_ARRIVE_Y 10
#define BUFF_ARRIVE_Z 25
#define WAIT_ARRIVE 40
inline bool has_arrived(int cnt_x, int cnt_y, int cnt_z)
{
	return ((cnt_x >= WAIT_ARRIVE) && (cnt_y >= WAIT_ARRIVE) && (cnt_z >= WAIT_ARRIVE));
}

#define STICK_ZERO 45
void ps_command()
{
	pspad.PsRead();
	ps::start = pspad.BUTTON.BIT.START && !pspad.PREV.BUTTON.BIT.START;
	ps::maru = pspad.BUTTON.BIT.MARU && !pspad.PREV.BUTTON.BIT.MARU;
	ps::sankaku = pspad.BUTTON.BIT.SANKAKU && !pspad.PREV.BUTTON.BIT.SANKAKU;
	ps::sikaku = pspad.BUTTON.BIT.SIKAKU && !pspad.PREV.BUTTON.BIT.SIKAKU;
	ps::batu = pspad.BUTTON.BIT.BATU && !pspad.PREV.BUTTON.BIT.BATU;
	ps::up = pspad.BUTTON.BIT.UP && !pspad.PREV.BUTTON.BIT.UP;
	ps::down = pspad.BUTTON.BIT.DOWN && !pspad.PREV.BUTTON.BIT.DOWN;
	ps::left_x = pspad.left_x;
	ps::left_y = pspad.left_y;
	ps::right_y = pspad.right_y;
	ps::R1 = pspad.BUTTON.BIT.R1;
	ps::R2 = pspad.BUTTON.BIT.R2;
	ps::L1 = pspad.BUTTON.BIT.L1;
	ps::L2 = pspad.BUTTON.BIT.L2;
	ps::select = pspad.BUTTON.BIT.SELECT;
	ps::R3 = pspad.BUTTON.BIT.R3;
	ps::L3 = pspad.BUTTON.BIT.L3;
	// 調整
	if (abs(ps::left_x) < STICK_ZERO) ps::left_x = 0;
	if (abs(ps::left_y) < STICK_ZERO) ps::left_y = 0;
	if (abs(ps::right_y) < STICK_ZERO) ps::right_y = 0;
	ps::left_x /= 100.0;
	ps::left_y /= 100.0;
	ps::right_y /= 100.0;
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
	Monitor_polar theta(1.4, 0, 0.06);
	Monitor_polar phi(1.5, 0, 0.005);

	int servo_duty;
	int fan_duty = 1020;
	int fan_adjust = 0;

	int x_adjust = 0;
	int y_adjust = 0;
	int z_adjust = 0;

	float now_t;
	bool is_first = true; // 最初のneutral指令
	bool is_shooting = false;

	Queue<Instruction> queue_inst;

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
	catcher.restart(inst.x, inst.y, inst.z, inst.slider);
	catcher.set_duration(inst.duration);
	catcher.set_mode(inst.coord, inst.acc, inst.slider);

	x.pos_now = INIT_X;
	y.pos_now = INIT_Y;
	z.pos_now = INIT_Z;

	slider.write(0);
	one_holder.write(0);
	work_wall.write(0);
	shooter.write(0);
	led_all(0);

	timer_shooting.reset();
	timer_shooting.stop();

	while(1){
		AdjustCycle(1000);
		ps_command();

		// コントローラで直接操作
		x.pos_adjust += ps::left_x;
		y.pos_adjust += ps::left_y;
		z.pos_adjust += ps::right_y;
		x_adjust = X_ADJUST;
		y_adjust = Y_ADJUST;
		z_adjust = Z_ADJUST;
		// limit
		if (inst.state == Mode::OwnArea) {
			z_adjust = limit(z_adjust, (int)(INIT_Z-inst.z), (int)(100-inst.z));
		}
		if (ps::maru) {
			inst.suction = (inst.suction == Mode::Hold ? Mode::Release : Mode::Hold);
			if (inst.state == Mode::ShootingBox) {
				if (inst.suction == Mode::Hold) {
					inst.suction = Mode::Release;
					queue_inst.push(stay_inst(inst.x+x_adjust, inst.y+y_adjust, inst.z+z_adjust, 0.5, Mode::Release, inst.slider));
	//				queue_inst.push(neutral_inst(1.5, Mode::Polar, Mode::Release));
				}
//				if (is_first) {
//					timer_shooting.reset();
//					timer_shooting.start();
//				}
				x.cnt_arrive = WAIT_ARRIVE*2;
				y.cnt_arrive = WAIT_ARRIVE*2;
				z.cnt_arrive = WAIT_ARRIVE*2;
			}
		}
		if (ps::R1 && ps::R2 && ps::L1 && ps::L2) {
			is_shooting = true;
			timer_shooting.reset();
			timer_shooting.start();
		}
		if (ps::select) {
			is_first = false;
		}
		one_holder.write(ps::select);
		// シューティングに関するリセット
		if (ps::R3 && ps::L3) {
			is_first = true;
			is_shooting = false;
			work_wall.write(0);
			timer_shooting.reset();
			timer_shooting.stop();
		}

		// コントローラからqueue_instにpush
		if (ps::start && inst.suction == Mode::Release) { // 強制的にneutralへ・startで1個取りモード
			if (is_first) {
				queue_inst.push(neutral_inst(1.5, inst.coord, inst.suction));
				queue_inst.push(go_fast_one_inst(2, Mode::NonLinearAcc));
			}
			else {
				queue_inst.pop();
				queue_inst.push(neutral_inst(1.5, inst.coord, inst.suction));
				queue_inst.push(neutral_inst(1.5, inst.coord, inst.suction));
				x.pos_adjust = 0;
				y.pos_adjust = 0;
				z.pos_adjust = 0;
				x.cnt_arrive = 0;
				y.cnt_arrive = 0;
				z.cnt_arrive = 0;
				inst = queue_inst.front();
				catcher.set_duration(inst.duration);
				catcher.set_mode(inst.coord, inst.acc, inst.slider);
				catcher.restart(inst.x, inst.y, inst.z, inst.slider);
				timer.reset();
				timer.start();
			}
		}
		if (ps::batu && inst.suction == Mode::Release) { // 自陣へ向かう
			// リトライとかで1個取りキャンセルするとき
			if (is_first) is_first = false;
			queue_inst.push(neutral_inst(1.5, Mode::Polar, Mode::Release));
			queue_inst.push(own_area_inst(X_OFFSET, 230, 140, Mode::NonLinearAcc));
		}
		if (ps::sankaku && inst.suction == Mode::Release) { // 共通エリアへ向かう
			if (is_first) is_first = false;
			queue_inst.push(neutral_inst(1.5, Mode::Polar, Mode::Release));
			queue_inst.push(common_area_inst(X_OFFSET, 500, 620, Mode::NonLinearAcc));
		}
		if (ps::sikaku && (inst.state == Mode::OwnArea || inst.state == Mode::CommonArea) && inst.suction == Mode::Hold) { // 集荷
			queue_inst.push(neutral_inst(2, Mode::Cartesian, Mode::Hold));
			if (is_first) {
				// 1個用の場所へ
				queue_inst.push(above_fast_one_box_inst(2, Mode::Polar, Mode::Hold));
			}
			else {
				queue_inst.push(above_box_front_inst(2.5, Mode::Polar, Mode::Hold));
			}
			x.cnt_arrive = WAIT_ARRIVE*2;
			y.cnt_arrive = WAIT_ARRIVE*2;
			z.cnt_arrive = WAIT_ARRIVE*2;
		}

		if (ps::up && inst.state == Mode::ShootingBox && !is_first) {
			queue_inst.push(above_box_front_inst(1, Mode::Cartesian, Mode::Hold));
		}
		if (ps::down && inst.state == Mode::ShootingBox && !is_first) {
			queue_inst.push(above_box_rear_inst(1, Mode::Cartesian, Mode::Hold));
		}

		if (ps::up && (inst.state == Mode::OwnArea || inst.state == Mode::CommonArea) && inst.suction == Mode::Hold) {
			fan_adjust += 50;
		}
		if (ps::down && (inst.state == Mode::OwnArea || inst.state == Mode::CommonArea) && inst.suction == Mode::Hold) {
			fan_adjust -= 50;
		}
		if (inst.suction == Mode::Release) {
			fan_adjust = 0;
		}

		now_t = timer.read();

		// 指令に従って次の位置を計算
		catcher.update_target(inst.x+x_adjust, inst.y+y_adjust, inst.z+z_adjust, inst.slider);
		catcher.calc_next();

		r.pos_next = limit(catcher.get_r_next(), 651.0, R_OFFSET);
		theta.pos_next = limit(catcher.get_theta_next(), M_PI, degree2rad(-39));
		phi.pos_next = limit(catcher.get_phi_next(), degree2rad(177), M_PI/2.0);

		// 次の位置に移動
		r.duty = motor_r.move_to(r.pos_next);
		theta.duty = motor_theta.move_to(theta.pos_next);
		phi.duty = motor_phi.move_to(phi.pos_next);
		// y方向スライド
		slider.write(inst.slider);
		// ファン
		if (inst.suction == Mode::Hold)	fan_duty = fan.on(1400 + fan_adjust);
		else fan_duty = fan.off();
		// 1個シューティング
//		if (is_first && timer_shooting.read() > 2) {
//			one_holder.write(1);
//			timer_shooting.stop();
//			is_first = false;
//		}
		// シューティング
		if (is_shooting) {
			work_wall.write(1);
			if (timer_shooting.read() > 1.5) shooter.write(1);
		}

		// 現在値取得
		r.pos_now = motor_r.get_now();
		theta.pos_now = motor_theta.get_now();
		phi.pos_now = motor_phi.get_now();

		// ハンドのサーボ
		if (is_first && (inst.state == Mode::ShootingBox || inst.state == Mode::Stay)) servo_duty = servo.keep(M_PI / 2.0);
		else servo_duty = servo.keep(phi.pos_now);

		polar2cartesian(r.pos_now, theta.pos_now, phi.pos_now, PHI_RADIUS,
				&(x.pos_now), &(y.pos_now), &(z.pos_now));
		x.pos_now += X_OFFSET;
		y.pos_now += Y_OFFSET;
		z.pos_now += Z_OFFSET;
		if (inst.slider == Mode::Forward) y.pos_now += SLIDER_OFFSET;

		x.cnt_arrive = limit(counter_update(x.cnt_arrive, x.pos_now, inst.x+x_adjust, BUFF_ARRIVE_X), WAIT_ARRIVE+10, 0);
		y.cnt_arrive = limit(counter_update(y.cnt_arrive, y.pos_now, inst.y+y_adjust, BUFF_ARRIVE_Y), WAIT_ARRIVE+10, 0);
		z.cnt_arrive = limit(counter_update(z.cnt_arrive, z.pos_now, inst.z+z_adjust, BUFF_ARRIVE_Z), WAIT_ARRIVE+10, 0);
		if (inst.state == Mode::Stay) {
			if (now_t < inst.duration) {
				x.cnt_arrive = 0;
				y.cnt_arrive = 0;
				z.cnt_arrive = 0;
			}
		}

		if (has_arrived(x.cnt_arrive, y.cnt_arrive, z.cnt_arrive)) {
			// まだ次の命令が残っている場合
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
				catcher.restart(inst.x, inst.y, inst.z, inst.slider);
				timer.reset();
				timer.start();
			}
		}

		pc.printf("now: %2.2f  q: %d  ", now_t, queue_inst.length());
		pc.printf("x: %4.1f->%4.1f   y: %4.1f->%4.1f  z: %4.1f->%4.1f  ",
				x.pos_now, inst.x+x_adjust, y.pos_now, inst.y+y_adjust, z.pos_now, inst.z+z_adjust);
		pc.printf("r: %4.1f->%4.1f  theta: %3.1f->%3.1f  phi: %3.1f->%3.1f  ",
				r.pos_now, r.pos_next,
				rad2degree(theta.pos_now), rad2degree(theta.pos_next),
				rad2degree(phi.pos_now), rad2degree(phi.pos_next));
//		pc.printf("duty: %1.4f %1.4f %1.4f  ", r.duty, theta.duty, phi.duty);
		pc.printf("servo: %4d  fan: %4d  ", servo_duty, fan_duty);
//		pc.printf("start:%d ", ps::start);
//		pc.printf("maru:%d sankaku:%d sikaku:%d batu:%d ", ps::maru, ps::sankaku, ps::sikaku, ps::batu);
//		pc.printf("up:%d down:%d ry:%d lx:%d ly:%d", ps::up, ps::down, ps::right_y, ps::left_x, ps::left_y);
		pc.printf("holder: %d  ", one_holder.read());
//		pc.printf("r3: %d  l3: %d  ", ps::R3, ps::L3);
		pc.printf("\r\n");
	}

	return 0;
}

