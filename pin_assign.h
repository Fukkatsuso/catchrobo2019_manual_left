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


Serial pc(USBTX, USBRX);

//Pspad ps;

FnkOut pwm_r(p21, p22);
PwmOut pwm_theta(p23);
PwmOut pwm_phi(p24);

DigitalOut slider(p26);

#define SERVO_MAX 2500
#define SERVO_MIN 500
#define SERVO_OFFSET_DEGREE 90.0
inline float degree2rad(float degree) { return degree / (2.0 * M_PI); }
//SoftPWM servo(p25);
HandServo<SoftPWM> servo(p25, (SERVO_MAX-SERVO_MIN)/(M_PI), degree2rad(SERVO_OFFSET_DEGREE));

//メインアームの歯
//θが75:15
//Φが180:30(モーター):42(エンコーダー)
//rのピッチ円直径は30mm

#define R_MM_PER_PULSE ((30.0 * M_PI) / 800.0)
#define R_OFFSET 0
Encoder enc_r(p9, p10, R_MM_PER_PULSE, R_OFFSET);

// 800pulse = theta:1/5回転 = theta:2PI/5rad
// rad/pulse = (2PI*1/5) / 800
#define THETA_RAD_PER_PULSE (2.0 * M_PI * (15 / 75) / 800.0)
#define THETA_OFFSET 0
Encoder enc_theta(p13, p14, THETA_RAD_PER_PULSE, THETA_OFFSET);

// 800pulse = phi:42/180回転 = phi:2PI*42/180rad
// rad/pulse = (2PI*42/180) / 800
#define PHI_RAD_PER_PULSE (2.0 * M_PI * (42.0 / 180) / 800.0)
#define PHI_OFFSET 0
Encoder enc_phi(p27, p28, PHI_RAD_PER_PULSE, PHI_OFFSET);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);


#endif /* PIN_ASSIGN_H_ */
