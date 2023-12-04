/***** DCBlock.cpp *****/

#include "DCBlock.h"

using namespace pultzLib;

// Constructor taking arguments for sample rate and table data
DCBlock::DCBlock(float frequency) {
	init(frequency);
}

void DCBlock::init(float frequency)
{
	frequency_ = frequency;
	coeff_ = 1.0f - (2.0f * M_PI * frequency_ / g_SampleRate);
	x_ = 0.0f;
	y_ = 0.0f;
	y_m1 = 0.0f;
	x_m1 = 0.0f;
}

// Set the frequency 
void DCBlock::setFrequency(float frequency) {
coeff_ = 1.0f - (2.0f * M_PI * frequency_ * g_SampleDur);
}

// Filter function
float DCBlock::process(float in) {
	x_ = in;
	y_ = x_ - x_m1 + coeff_ * y_;
	x_m1 = x_;
			
	return y_;
}
