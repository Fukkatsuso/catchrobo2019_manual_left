/*
 * pspad.c
 *
 *  Created on: 2012/12/21
 *      Author: KORA
 */

#include "mbed.h"
#include "pspad.h"

//----------------------------------------------------------
// PS PAD
//----------------------------------------------------------
#define PS_DAT    datpin        //400Ωほどのプルアップ必要
#define PS_CMD_H  cmdpin = 1
#define PS_CMD_L  cmdpin = 0
#define PS_SEL_H  selpin = 1
#define PS_SEL_L  selpin = 0
#define PS_CLK_H  clkpin = 1
#define PS_CLK_L  clkpin = 0

//----------------------------------------------------------
// 動作周波数
//----------------------------------------------------------
#define PS_FREQ    100//125         // パッドの動作周波数    (kHz)
                                    // タイマはこの２倍の周波数で動かす
                                    // 本来250kHzだが変更しても問題ない
                                    // DAT線のプルアップ抵抗が高すぎる場合は速度を落とすこと

#define TM_PERI    (500/PS_FREQ)    // タイマの１クロックに要する時間(us)
                                    // タイマの２クロックをパッドの１クロックとしている

//----------------------------------------------------------
// constructor
//----------------------------------------------------------
Pspad::Pspad(PinName dat, PinName cmd, PinName sel, PinName clk) : datpin(dat), cmdpin(cmd), selpin(sel), clkpin(clk)
{
    datpin.mode(PullUp);
    // GPIOの出力値を設定する
    PS_CMD_H;               // CMD を1に
    PS_CLK_H;               // CLK を1に
    PS_SEL_H;               // SEL を1に
    
    // タイマの設定
    tm.reset();
}

//----------------------------------------------------------
// プレステ用ゲームパッドとの１バイトの通信
// 引数
//  int send : 送信データ（0～255）
// 戻り値
//  int 受信データ（0～255）
//----------------------------------------------------------
uint8_t Pspad::PsComm( uint8_t send_data )
{
    uint8_t i;
    uint8_t recv_data = 0;

    // カウント開始
    tm.start();

    for(i=0; i<8; i++)
    {

        // 送信データ
        if(send_data & 0x01){
            PS_CMD_H;
        }else{
            PS_CMD_L;
        }
        // クロックをLOWに
        PS_CLK_L;
        // 待機
        while(tm.read_us()/TM_PERI <= 2*i);

        // クロックをHIGHに
        PS_CLK_H;
        // 受信データ
        recv_data |= (PS_DAT<<i);
        // 待機
        while(tm.read_us()/TM_PERI <= 2*i+1);

        // 送信データを１ビットずらす
        send_data >>= 1;
    }

    // CMDをHIGHに戻す
    PS_CMD_H;

    // ５クロックほど待機
    while(tm.read_us()/TM_PERI <= 2*8+2*5);

    // カウント停止 及び リセット
    tm.stop();
    tm.reset();

    return recv_data;
}

//----------------------------------------------------------
// プレステ用ゲームパッドからのデータの取得
// 変数
//  uint32_t key      ボタンの押下状態をbitmapで返す。
//  int8_t   right_x  -128～127 の値を返す。アナログスティック中立時は０
//  int8_t   right_y  -128～127 の値を返す。アナログスティック中立時は０
//  int8_t   left_x   -128～127 の値を返す。アナログスティック中立時は０
//  int8_t   left_y   -128～127 の値を返す。アナログスティック中立時は０
//----------------------------------------------------------
void Pspad::PsRead(void)
{
    uint8_t i, len;
    uint8_t rcv[20];

    //BUTTON.WORD = 0;
    right_x = 0;  // アナログスティック中立
    right_y = 0;  // アナログスティック中立
    left_x  = 0;  // アナログスティック中立
    left_y  = 0;  // アナログスティック中立

    // SELをLOWに
    PS_SEL_L;

    // 少し待機
    tm.start();
    while(tm.read_us() <=  40);      // 40usでも動く模様
    tm.stop();
    tm.reset();

    // 通信開始
    rcv[0] = PsComm(0x01);          // PADはコマンド'01h'を検出して動作を開始する
    rcv[1] = PsComm(0x42);          // 受信データの下位４ビットが転送バイト数の半分の数を表す
    len = (rcv[1] & 0x03) * 2;      // 転送バイト数を求める
    unsigned char data = 0x00;
    if(VIBRATE)data = 0x41;
    for(i=0;i<len+1;i++){
        rcv[i+2] = PsComm(data);//PsComm(0x00);    // PADの状態を受信
    }

    // SELをHIGHに
    PS_SEL_H;

    // PADの状態を返す
    PREV.BUTTON.WORD = BUTTON.WORD;
    BUTTON.WORD = ((rcv[3]<<8) | rcv[4]) ^ 0xFFFF;
    
    PREV.right_x = right_x;
    PREV.right_y = right_y;
    PREV.left_x = left_x;
    PREV.left_y = left_y;
    if(len >= 6){   //Convert range to -128~127
        ANALOG_MODE = 1;
        right_x = rcv[5] - 128;
        right_y = -(rcv[6] - 128 +1);
        left_x  = rcv[7] - 128;
        left_y  = -(rcv[8] - 128 +1);
    }else{
        ANALOG_MODE = 0;
        right_x = 0;
        right_y = 0;
        left_x  = 0;
        left_y  = 0;
    }
}
