/*
 * pspad.h
 *
 *  Created on: 2012/12/21
 *      Author: USER
 */

#ifndef PSPAD_H_
#define PSPAD_H_

/*--------PSPADのアクセスの構造体-------
PS--.PsRead()
|---.right_x
|---.right_y
|---.left_x
|---.left_y;
|---.VIBRATE;
|---.ANALOG_MODE;
|---.BUTTON .WORD
|       |---.BIT.L2
|             |-.R2
|             |-.L1
|             |-.R1
|             |-.SANKAKU
|             |-.MARU
|             |-.BATU
|             |-.SIKAKU
|             |-.SELECT
|             |-.L3
|             |-.R3
|             |-.START
|             |-.UP
|             |-.RIGHT
|             |-.DOWN
|             --.LEFT
----.PERV--.right_x
       |---.right_y
       |---.left_x
       |---.left_y;
       ----.BUTTON .WORD
               |---.BIT.L2
                     |-.R2
                     |-.L1
                     |-.R1
                     |-.SANKAKU
                     |-.MARU
                     |-.BATU
                     |-.SIKAKU
                     |-.SELECT
                     |-.L3
                     |-.R3
                     |-.START
                     |-.UP
                     |-.RIGHT
                     |-.DOWN
                     --.LEFT
---------------------------------------*/

//-------------------------------------------------------------------------
// Class
//-------------------------------------------------------------------------
class Pspad
{
public:
    Pspad(PinName dat, PinName cmd, PinName sel, PinName clk);
    void PsRead(void);

    union {
        uint32_t WORD;
        struct {
            unsigned char L2       :1;
            unsigned char R2       :1;
            unsigned char L1       :1;
            unsigned char R1       :1;

            unsigned char SANKAKU  :1;
            unsigned char MARU     :1;
            unsigned char BATU     :1;
            unsigned char SIKAKU   :1;

            unsigned char SELECT   :1;
            unsigned char L3       :1;
            unsigned char R3       :1;
            unsigned char START    :1;

            unsigned char UP       :1;
            unsigned char RIGHT    :1;
            unsigned char DOWN     :1;
            unsigned char LEFT     :1;
        } BIT;
    } BUTTON;
    int8_t right_x, right_y, left_x, left_y;
    unsigned char VIBRATE;
    unsigned char ANALOG_MODE;
    struct {
        union {
            uint32_t WORD;
            struct {
                unsigned char L2       :1;
                unsigned char R2       :1;
                unsigned char L1       :1;
                unsigned char R1       :1;

                unsigned char SANKAKU  :1;
                unsigned char MARU     :1;
                unsigned char BATU     :1;
                unsigned char SIKAKU   :1;

                unsigned char SELECT   :1;
                unsigned char L3       :1;
                unsigned char R3       :1;
                unsigned char START    :1;

                unsigned char UP       :1;
                unsigned char RIGHT    :1;
                unsigned char DOWN     :1;
                unsigned char LEFT     :1;
            } BIT;
        } BUTTON;
        int8_t right_x, right_y, left_x, left_y;
    } PREV;

private:
    uint8_t PsComm(uint8_t send_data);
    Timer tm;
    DigitalIn  datpin;
    DigitalOut cmdpin;
    DigitalOut selpin;
    DigitalOut clkpin;
};

#endif /* PSPAD_H_ */
