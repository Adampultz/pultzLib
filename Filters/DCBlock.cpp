/***** DCBlock.cpp *****/

#include "DCBlock.h"
#include <cmath>

// Constructor taking arguments for sample rate and table data
DCBlock::DCBlock(float frequency, float sampleRate) {
	setup(frequency, sampleRate);
}

void DCBlock::setup(float frequency, float sampleRate)
{
	frequency_ = frequency;
	inverseSampleRate_ = 1.0f / sampleRate;
	coeff_ = 1.0f - (2.0f * M_PI * frequency_ / sampleRate);
	x_ = 0.0f;
	y_ = 0.0f;
	y_m1 = 0.0f;
	x_m1 = 0.0f;
}

// Set the frequency 
void DCBlock::setFrequency(float frequency) {
coeff_ = 1.0f - (2.0f * M_PI * frequency_ *  inverseSampleRate_);
}


// Filter function
float DCBlock::process(float in) {
	x_ = in;
	y_ = x_ - x_m1 + coeff_ * y_;
	x_m1 = x_;
			
	return y_;
}
