/*
 * BLdriver.cpp
 *
 *  Created on: 2016/10/12
 *      Author: Wataru Hirota
 */

#include "BLdriver.h"

Serial BLserial ( p13, p14 );

void BL_driver::set_serial(){
	BLserial.baud( 115200/*19600*//*57600*/ );
	//BLserial.format(8,Serial::Even);
	BLserial.format(8,Serial::None);
}

long BL_driver::make16(int varhigh, int varlow){
    return ((varhigh&0xff)*0x100+(varlow&0xff));
}

long BL_driver::make8(int var,int offset){
    return ((var>>(offset*8))&0xff);

}
long BL_driver::bit_set(int var, int bit){
	return var |= (1<<bit);
}


void BL_driver::int_send_normal(int id,short ab,BYTE io,long pwm){
	BYTE data[3] = {0x80,0,0};

	id&=0x0f;
	io&=0x03;
//	pwm&=0x03ff;
	if(pwm>1023)
		pwm=1023;
	data[0]|=id<<3;
	if(ab)
		bit_set(data[0],2);	//data[0]|=0x04と同じ
	BLserial.putc(data[0]);
	data[1]=io<<3;
	data[1]|=make8(pwm,1)<<1;
	if(make8(pwm,0)&0x80)
		data[1]|=0x01;
	data[2]=make8(pwm,0)&0x7f;

	BLserial.putc(data[1]);
	BLserial.putc(data[2]);	//86.8us/byte
	BLserial.putc(CheckSum());
}


void BL_driver::send_normal(BYTE id,BYTE ab,BYTE io,long pwm){
	BYTE data[3] = {0x80,0,0};

	id&=0x0f;
	io&=0x03;
//	pwm&=0x03ff;
	if(pwm>1023)
		pwm=1023;
	data[0]|=id<<3;
	if(ab)
		bit_set(data[0],2);	//data[0]|=0x04と同じ
	BLserial.putc(data[0]);
	data[1]=io<<3;
	data[1]|=make8(pwm,1)<<1;
	if(make8(pwm,0)&0x80)
		data[1]|=0x01;
	data[2]=make8(pwm,0)&0x7f;

	BLserial.putc(data[1]);
	BLserial.putc(data[2]);	//86.8us/byte
	BLserial.putc(CheckSum());
}

void BL_driver::send_servo(BYTE id,short ab,long position){
	BYTE data[3] = {0x80,0x20,0};

	id&=0x0f;
//	position&=0x03ff;
	if(position>1023)
		position=1023;
	data[0]|=id<<3;
	if(ab)
		bit_set(data[0],0x02);	//data[0]|=0x04と同じ
	BLserial.putc(data[0]);
	data[1]|=make8(position,1)<<1;
	if(make8(position,0)&0x80)
		data[1]|=0x01;
	data[2]=make8(position,0)&0x7f;
	BLserial.putc(data[1]);
	BLserial.putc(data[2]);	//86.8us/byte
	BLserial.putc(CheckSum());
}

void BL_driver::send_bl_normal(BYTE id,BYTE ab,BYTE io,long pwm){
	BYTE data[3] = {0x82,0,0};

	id&=0x0f;
	io&=0x03;
//	pwm&=0x03ff;
	if(pwm>1023)
		pwm=1023;
	data[0]|=id<<3;
	if(ab)
		bit_set(data[0],2);	//data[0]|=0x04と同じ
	BLserial.putc(data[0]);
	data[1]=io<<3;
	data[1]|=make8(pwm,1)<<1;
	if(make8(pwm,0)&0x80)
		data[1]|=0x01;
	data[2]=make8(pwm,0)&0x7f;

	BLserial.putc(data[1]);
	BLserial.putc(data[2]);	//86.8us/byte
	BLserial.putc(CheckSum());
}

int BL_driver::send_servo_Retry(BYTE id,short ab,long position){
	BYTE data[4] = {0x80,0x28,0,0};

	id&=0x0f;
//	position&=0x03ff;
	if(position>1023)
		position=1023;
	data[0]|=id<<3;
	if(ab)
		bit_set(data[0],0x02);	//data[0]|=0x04と同じ
	data[1]|=make8(position,1)<<1;
	if(make8(position,0)&0x80)
		data[1]|=0x01;
	data[2]=make8(position,0)&0x7f;
	data[3] = CheckSum();

	return send_retry_(data);
}

void BL_driver::send_servo_speed(BYTE id,short ab,BYTE speed){
	BYTE data[3] = {0x81,0x60,0};

	id&=0x0f;
	data[0]|=id<<3;
	if(ab)
		bit_set(data[0],0x02);	//data[0]|=0x04と同じ
	BLserial.putc(data[0]);
	if(speed&0x80)
		bit_set(data[1],0);
	data[2]=speed&0x7f;
	BLserial.putc(data[1]);
	BLserial.putc(data[2]);	//86.8us/byte
	BLserial.putc(CheckSum());
}

///
//	return 0xffff 受信失敗
///

void BL_driver::send_reset(BYTE id,short ab){
	BYTE data[3] = {0x81,0x40,0};

	id&=0x0f;
	data[0]|=id<<3;

	BLserial.putc(data[0]);
	BLserial.putc(data[1]);
	BLserial.putc(data[2]);	//86.8us/byte
	BLserial.putc(CheckSum());

}

long BL_driver::recv_servo_data(BYTE id,BYTE adr){
	BYTE data[4] = {0x81,0x20,0,0};
	long input;

//	output_low(SerboMISS);
//	output_high(SerboOK);

	id&=0x0f;
	data[0]|=id<<3;
	if(adr&0x40)
		bit_set(data[0],0x02);	//data[0]|=0x04と同じ
	adr&=0x1f;
	data[1]|=adr;
	data[3]=CheckSum();

	if(!send_retry_(data))
		return 0xffff;

	//disable_interrupts(INT_RDA);	//受信 割り込み不可
	LPC_UART3->LCR&=(1<<6);
	input=make16(int_recv[0]&0x7f,1)>>1;
	input|=int_recv[1];
	//enable_interrupts(INT_RDA);	//受信 割り込み許可
	LPC_UART3->LCR|=(1<<6);
	if(adr == VOLT_DATA)
		input=input*38 + (input>>3);
	//output_low(SerboMISS);
	//output_low(SerboOK);

	if(adr == REST_TIME)
		input=input*16/10;
	//output_low(SerboMISS);
	//output_low(SerboOK);

	return input;

}
/*
BYTE send_retry_(BYTE data[]){

	BYTE time_out;
	long t,sum,n;

	set_serbo_led(1);	//OK

	for(n=0;n<RETRY;n++){
		int_r=0;
		BL.putc(data[0]);
		BL.putc(data[1]);
		BL.putc(data[2]);	//86.8us/byte
		int_r=1;
		delay_us(310);	//86.8*3+50us
		BL.putc(data[3]);
		delay_us(347);		//86.8*4
		time_out=0;

		t=0;
		while(int_r==1 && ++t<400){	//2000us以内に受信しない場合
			//delay_cycles(16);		//(5us)
			wait_us(5);
		}

		if(time_out == 0){
			//disable_interrupts(INT_RDA);	//受信 割り込み不可
			sum=int_recv[1];
			//enable_interrupts(INT_RDA);	//受信 割り込み許可
			if(sum==data[3])	//チェックサムが同じなら
				break;
		}
	}
	if(time_out == 1){
		set_serbo_led(0);	//MISS
		return FALSE;
	}

	set_serbo_led(3);		//消灯

	return TRUE;

}
*/

//#int_rda	//受信割り込み
//void int_usert(void){
//	static BYTE d[2],index=0;

	///	エラー処理
//	if(RCSTA&0x04){		//フレーミングエラー(Stop bitの検出失敗)
//		output_high(SerboMISS);
//		index=0x0;
//	}

//	if(RCSTA&0x02){		//オーバーランエラー(データが上書きされた)
//		output_high(SerboMISS);
//		RCSTA&=0xef;		//継続受信不可
//		RCSTA|=0x10;		///継続受信許可
//		index=0x0;
//	}
//	if(RS232_ERRORS&0x01){		//パリティーエラー
//		output_high(SerboMISS);
//		index=0x0;
//	}
//	if(int_r==0){
//		index=0;
//		getc();
//		return;
//	}

	///	受信処理
//	if(index==0){
//		d[0]=getc();
//		if(d[0]&0x80){			//Start bit?
//			output_high(SerboOK);
//			index++;
//		}else{
//			output_low(SerboOK);
//			output_high(SerboMISS);
//		}
//	}else if(index==1){
//		d[1]=getc();
//		if(!(d[1]&0x80)){

//			int_recv[0]=d[0];
//			int_recv[1]=d[1];
//			int_r=0;

//			output_low(SerboMISS);
//			output_low(SerboOK);
//			index=0;
//		}else{
//			output_high(SerboMISS);
//			output_low(SerboOK);
//			index=0;
//		}
//	}

//}

//;*******************************************
//; 状態表示Serbo LED サブルーチン 1:ok 0:miss 2:double 3:off
//;*******************************************
/*
void set_servo_led(BYTE color){
	BYTE i;
	switch(color){
	case 1:					//OKらんぷ点灯
		output_high(SERBOOK);
		output_low(SERBOMISS);
		break;
	case 0:					//miss ランプ点灯
		output_low(SERBOOK);
		output_high(SERBOMISS);
		break;
	case 2:					//両方点灯
		output_high(SERBOOK);
		output_high(SERBOMISS);
		break;
	case 10:					//両方点滅
		for(i=0;i<2;i++){
			output_high(SERBOOK);
			output_high(SERBOMISS);
			delay_ms(250);
			output_low(SERBOOK);
			output_low(SERBOMISS);
			delay_ms(250);
		}
		break;
	default:
	case 3:
		output_low(SERBOOK);
		output_low(SERBOMISS);
	}
}

long conv_servo_data(long data){

	if(data<=720){	//ゼロ基準へ
		data=0;
	}else{
		data-=720;
	}

	data*=2;				//0-460 →0-920
	data+=50;		//マージン設定 50-970

	if(data<100)
		data=100;		//100-970
	if(data>923)		//max923設定 100-923
		data=923;

	return data;
}
*/
