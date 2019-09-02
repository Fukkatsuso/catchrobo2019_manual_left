/*
 * encoder.cpp
 *
 *  Created on: 2019/08/28
 *      Author: mutsuro
 */


#include "encoder.h"


Encoder::Encoder(PinName channelA, PinName channelB, const float pulse2mm, float offset = 0):
	QEI(channelA, channelB, NC, 624, QEI::X4_ENCODING), pulse2mm(pulse2mm)
{
	set_offset(offset);
	QEI::reset();
}


void Encoder::set_offset(float offset)
{
	this->offset = offset;
}


float Encoder::get_distance()
{
	int pulse = QEI::getPulses();
	float dist = offset + (float)pulse * pulse2mm;
	return dist;
}
