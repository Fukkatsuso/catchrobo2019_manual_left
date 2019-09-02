/*
// 		Moter Driver_K v2.x受信機用受信ライブラリ
// 		2013/02/22作成

//　		木村建斗 		k_kimu1
//		k.kimu.j.souken@gmail.com
 */

#ifndef BLDRIVER_H_
#define BLDRIVER_H_
#include "mbed.h"


#define	ACH			0			//Aチャンネル（基本使わない）
#define	BCH			1			//Bチャンネル（こっちがメイン）
#define	CW			0x01		//正転(0~1023)だいたいMAX400
#define	CCW			0x02		//逆転(0~1023)
#define	STOP		0x00		//フリー
#define	BRAKE		0x03		//ブレーキ(0~1023)

#define	ID0			0x00		//
#define	ID1			0x01		//
#define	ID2			0x02		//
#define	ID3			0x03		//
#define	ID4			0x04		//
#define	ID5			0x05		//
#define	ID6			0x06		//
#define	ID7			0x07		//
#define	ID8			0x08		//
#define	ID9			0x09		//
#define	ID10		0x0a		//
#define	ID11		0x0b		//
#define	ID12		0x0c		//
#define	ID13		0x0d		//
#define	ID14		0x0e		//
#define	IDALL		0x0f		//

#define	ACH_DATA		0x00
#define	BCH_DATA		0x40
#define	VOLT_DATA		0x01
#define	VERSION_DATA	0x02
#define	MOTOR_POSI	0x03
#define	REST_TIME		0x04

#define BYTE int

//;*******************************************
//; サーボ関数
//;*******************************************
///// RS232C関連
//#USE RS232(BAUD=115200, XMIT=PIN_C6 ,RCV=PIN_C7 , BITS=8 , PARITY=E , ERRORS)
#define CheckSum() (data[0]+data[1]+data[2])&0x7f

extern Serial BLserial;

class BL_driver{
	private:
		//Serial BLserial ( serial_tx , serial_rx );
	public:
	//MD受信データ
	BYTE int_recv[2];		//受信データ
	short int_r=0;			//受信待ち

	void set_serial();		//main内で一回書く//シリアル通信の設定


	long make16(int varhigh, int varlow);
	long make8(int var,int offset);
	long bit_set(int var, int bit);


	void set_serbo_led(BYTE color);

	/*------------------------------------------------------
	 * この2つを中心に使う。
	 * id　：　動かすBLMDをID0~ID14で指定
	 * ab : チャンネルの設定。基本はBCH
	 * io : CW		:正回転。BLの後ろから見て右回り
	 * 		CCW		:逆回転。BLの後ろから見て左回り
	 * 		STOP	:フリー。自然に減速する。
	 * 		BRAKE	:ブレーキ。モーターの回転を止める。
	 *pwm : CW,CCW,BRAKEの強さ。0~1023。
	 -------------------------------------------------------*/
	void send_servo(BYTE id,short ab,long position);		//位置サーボモード(ID,BCH,0~1023)
	void send_bl_normal(BYTE id,BYTE ab,BYTE io,long pwm);	//BLモード	(ID,BCH,io,0~1023),id:md用

	/*-------------------------------------------------
	 * send_nomal AとCにつないでブラシモード。
	 * chはACHを指定
	 * -----------------------------------------------*/
	void send_normal(BYTE id,BYTE ab,BYTE io,long pwm);
	void send_servo_speed(BYTE id,short ab,BYTE speed);
	long conv_serbo_data(long data);
	void int_send_normal(BYTE id,short ab,BYTE io,long pwm);
	long recv_servo_data(BYTE id,BYTE adr);
	void send_reset(BYTE id,short ab);
	int send_servo_Retry(BYTE id,short ab,long position);
	BYTE send_retry_(BYTE data[]);


};


#endif /* BLDRIVER_H_ */
