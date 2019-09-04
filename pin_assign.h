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

#define R_MM_PER_PULSE 10
#define R_OFFSET 0
Encoder enc_r(p9, p10, R_MM_PER_PULSE, R_OFFSET);

#define THETA_MM_PER_PULSE 10
#define THETA_OFFSET 0
Encoder enc_theta(p13, p14, THETA_MM_PER_PULSE, THETA_OFFSET);

#define PHI_MM_PER_PULSE 10
#define PHI_OFFSET 0
Encoder enc_phi(p27, p28, PHI_MM_PER_PULSE, PHI_OFFSET);

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);


#endif /* PIN_ASSIGN_H_ */
