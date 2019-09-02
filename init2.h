/*
 * init2.h
 *
 *  Created on: 2018/09
 *      Author: mutsuro
 */
#ifndef INIT2_H_
#define INIT2_H_

#include "mbed.h"

#define CYCLE 5000 //動作周期(us)


Timer AdCycle; //AdjustCycleで使うタイマ

extern DigitalOut led4;


/*
 * 関数名	AdjustCycle
 *
 * 用途		マイコンの動作周期を調整する
 *
 * 引数		int t_us:目的の動作周期(us)
 *
 * 戻り値		なし
 *
 * 備考		関数実行時、前の実行時からt_us経っていない場合、t_us経つまで待つ
 * 			すでにt_us経っている場合、led3を点灯する
 */
void AdjustCycle(int t_us){
    if(AdCycle.read_us() == 0) AdCycle.start();

    if(AdCycle.read_us()>t_us){
    	led4=1;
//    	pc.printf("AdCycle=%dus\r\n",AdCycle.read_us());
    }
    else {
    	led4=0;
//    	pc.printf("AdCycle=%dus\r\n",AdCycle.read_us());
    }
    while(AdCycle.read_us()<=t_us);
    AdCycle.reset();
}


#endif /* INIT2_H_ */

