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

#define X_ADJUST (x.pos_adjust/4)
#define Y_ADJUST (y.pos_adjust/4)
#define Z_ADJUST (z.pos_adjust/4)

#define BUFF_ARRIVE_X 15 // [mm]
#define BUFF_ARRIVE_Y 10
#define BUFF_ARRIVE_Z 15
#define WAIT_ARRIVE 80
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
	ps::up = pspad.BUTTON.BIT.UP;
	ps::down = pspad.BUTTON.BIT.DOWN;
	ps::left_x = pspad.left_x;
	ps::left_y = pspad.left_y;
	ps::right_y = pspad.right_y;
	ps::R1 = pspad.BUTTON.BIT.R1;
	ps::R2 = pspad.BUTTON.BIT.R2;
	ps::L1 = pspad.BUTTON.BIT.L1;
	ps::L2 = pspad.BUTTON.BIT.L2;
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
	Monitor_polar theta(0.9, 0, 0.06);
	Monitor_polar phi(0.6, 0, 0.01);

	int servo_duty;
	int fan_duty = 1020;

	float now_t;
	bool is_first = true; // 最初のneutral指令

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

	slider.write(0);
	led_all(0);

	while(1){
		AdjustCycle(1000);
		ps_command();

		// コントローラで直接操作
		x.pos_adjust += ps::left_x;
		y.pos_adjust += ps::left_y;
		z.pos_adjust += ps::right_y;
//		if (ps::up) inst.slider = Mode::Forward;
//		else if (ps::down) inst.slider = Mode::Backward;
		if (ps::maru) {
			inst.suction = Mode::Hold;
		}
		if (ps::R1 && ps::R2 && ps::L1 && ps::L2) {
			// shooting
		}

		// コントローラからqueue_instにpush
		if (ps::start && inst.suction == Mode::Release) { // neutralへ・最初の1回は1個取り
			queue_inst.push(neutral_inst(2, inst.coord, inst.suction));
			if (is_first) {
				for (int i=0; i<FAST_ONE_GO_INST_NUM; i++) queue_inst.push(fast_one_go_inst[i]);
			}
		}
		if (ps::batu && inst.suction == Mode::Release) { // 自陣へ向かう
			queue_inst.push(neutral_inst(1.5, Mode::Polar, Mode::Release));
			queue_inst.push(own_area_inst(220, 120, 160, Mode::NonLinearAcc, Mode::Backward));
		}
		if (ps::sankaku && inst.suction == Mode::Release) { // 共通エリアへ向かう
			queue_inst.push(neutral_inst(1.5, Mode::Polar, Mode::Release));
			queue_inst.push(common_area_inst(220, 600, INIT_Z, Mode::NonLinearAcc));
		}
		if (ps::sikaku && (inst.state == Mode::OwnArea || inst.state == Mode::CommonArea) && inst.suction == Mode::Hold) { // 集荷
			if (is_first) {
				// 1個用の場所へ
				for (int i=0; i<FAST_ONE_RETURN_INST_NUM; i++) queue_inst.push(fast_one_return_inst[i]);
				is_first = false;
			}
			else {
				for (int i=0; i<PUT_WORK_INST_NUM; i++) queue_inst.push(put_work_inst[i]);
			}
			x.cnt_arrive = WAIT_ARRIVE*2;
			y.cnt_arrive = WAIT_ARRIVE*2;
			z.cnt_arrive = WAIT_ARRIVE*2;
		}

		now_t = timer.read();

		// 指令に従って次の位置を計算
		catcher.update_target(inst.x+X_ADJUST, inst.y+Y_ADJUST, inst.z+Z_ADJUST, inst.slider);
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
		// ファン
		if (inst.suction == Mode::Hold)	fan_duty = fan.on(1300);
		else fan_duty = fan.off();

		// 現在値取得
		r.pos_now = motor_r.get_now();
		theta.pos_now = motor_theta.get_now();
		phi.pos_now = motor_phi.get_now();

		// ハンドのサーボ
		servo_duty = servo.keep(phi.pos_now);

		polar2cartesian(r.pos_now, theta.pos_now, phi.pos_now, PHI_RADIUS,
				&(x.pos_now), &(y.pos_now), &(z.pos_now));
		x.pos_now += X_OFFSET;
		y.pos_now += Y_OFFSET;
		z.pos_now += Z_OFFSET;
		if (inst.slider == Mode::Forward) y.pos_now += SLIDER_OFFSET;

		x.cnt_arrive = limit(counter_update(x.cnt_arrive, x.pos_now, inst.x+X_ADJUST, BUFF_ARRIVE_X), WAIT_ARRIVE+10, 0);
		y.cnt_arrive = limit(counter_update(y.cnt_arrive, y.pos_now, inst.y+Y_ADJUST, BUFF_ARRIVE_Y), WAIT_ARRIVE+10, 0);
		z.cnt_arrive = limit(counter_update(z.cnt_arrive, z.pos_now, inst.z+Z_ADJUST, BUFF_ARRIVE_Z), WAIT_ARRIVE+10, 0);
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
				x.pos_now, inst.x+X_ADJUST, y.pos_now, inst.y+Y_ADJUST, z.pos_now, inst.z+Z_ADJUST);
		pc.printf("r: %4.1f->%4.1f  theta: %3.1f->%3.1f  phi: %3.1f->%3.1f  ",
				r.pos_now, r.pos_next,
				rad2degree(theta.pos_now), rad2degree(theta.pos_next),
				rad2degree(phi.pos_now), rad2degree(phi.pos_next));
//		pc.printf("duty: %1.4f %1.4f %1.4f  ", r.duty, theta.duty, phi.duty);
		pc.printf("servo: %4d  fan: %4d  ", servo_duty, fan_duty);
//		pc.printf("start:%d ", ps::start);
//		pc.printf("maru:%d sankaku:%d sikaku:%d batu:%d ", ps::maru, ps::sankaku, ps::sikaku, ps::batu);
//		pc.printf("up:%d down:%d ry:%d lx:%d ly:%d", ps::up, ps::down, ps::right_y, ps::left_x, ps::left_y);
		pc.printf("\r\n");
	}

	return 0;
}

