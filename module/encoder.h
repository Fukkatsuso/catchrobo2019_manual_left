/*
 * encoder.h
 *
 *  Created on: 2019/08/28
 *      Author: mutsuro
 */

#ifndef MODULE_ENCODER_H_
#define MODULE_ENCODER_H_

#include "mbed.h"
#include "./QEI.h"


class Encoder : public QEI
{
public:
	Encoder(PinName channelA, PinName channelB, const float pulse2mm, float offset);
	void set_offset(float offset);
	float get_distance();

private:
	const float pulse2mm;
	float offset;
};


#endif /* MODULE_ENCODER_H_ */
