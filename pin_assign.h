/*
 * pin_assign.h
 *
 *  Created on: 2019/08/28
 *      Author: mutsuro
 */

#ifndef PIN_ASSIGN_H_
#define PIN_ASSIGN_H_

#include "mbed.h"
#include "pspad.h"
#include "module/encoder.h"
#include "module/SoftPWM.h"
#include "module/FnkOut.h"
#include "module/hand_servo.h"
#include "module/fan.h"


Serial pc(USBTX, USBRX);

Pspad pspad(p12, p11, p14, p13);
// ボタンの値を入れておく変数
namespace ps
{
int start;
int maru, batu, sankaku, sikaku;
int up, down;
int left_x, left_y, right_y;
int R1, R2, L1, L2;
}

FnkOut pwm_r(p24, p25);
PwmOut pwm_theta(p23);
PwmOut pwm_phi(p22);

DigitalOut slider(p5);
DigitalOut one_holder(p6);
DigitalOut work_holder(p20);
DigitalOut shooter(p8);

#define SERVO_OFFSET (M_PI/2.0)
inline float degree2rad(float degree) { return M_PI * degree / 180.0; }
inline float rad2degree(float rad) { return 180.0 * rad / M_PI; }
//HandServo<SoftPWM> servo(p26, 0, M_PI/2.0, 950, 1800, SERVO_OFFSET);
HandServo<SoftPWM> servo(p26, 0, M_PI/2.0, 900, 1850, SERVO_OFFSET);

Fan<SoftPWM> fan(p21);

//メインアームの歯
//Φが75:15
//θが180:30(モーター):42(エンコーダー)
//rのピッチ円直径は30mm

#define R_MM_PER_PULSE ((30.0 * M_PI) / 800.0)
#define R_OFFSET 112.0 //[mm]
Encoder enc_r(p15, p16, R_MM_PER_PULSE, R_OFFSET);

// 800pulse = phi:1/5回転 = phi:2PI/5rad
// rad/pulse = (2PI*1/5) / 800
#define PHI_RAD_PER_PULSE (2.0 * M_PI * (15.0 / 75.0) / 800.0)
#define PHI_OFFSET degree2rad(90)
Encoder enc_phi(p10, p9, PHI_RAD_PER_PULSE, PHI_OFFSET);

// 800pulse = theta:42/180回転 = theta:2PI*42/180rad
// rad/pulse = (2PI*42/180) / 800
#define THETA_RAD_PER_PULSE (2.0 * M_PI * (42.0 / 180) / 800.0)
#define THETA_OFFSET 0
Encoder enc_theta(p28, p27, THETA_RAD_PER_PULSE, THETA_OFFSET);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);


#endif /* PIN_ASSIGN_H_ */
