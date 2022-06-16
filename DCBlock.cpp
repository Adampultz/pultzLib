/***** DCBlock.cpp *****/

#include "DCBlock.h"
#include <cmath>

DCBlock::DCBlock(float frequency, float sampleRate) {
	setup(frequency, sampleRate);
}

void DCBlock::setup(float frequency, float sampleRate)
{
	frequency_ = frequency;
	inverseSampleRate_ = 1.0f / sampleRate;
	coeff_ = expf(-2.0f * (float)M_PI * (frequency_ * inverseSampleRate_));
	x_ = 0.0f;
	y_ = 0.0f;
	y_m1 = 0.0f;
	x_m1 = 0.0f;
}

void DCBlock::setFrequency(float frequency) {
coeff_ = expf(-2.0f * (float)M_PI * (frequency * inverseSampleRate_));
}

float DCBlock::process(float in) {
	x_ = in;
	y_ = x_ - x_m1 + coeff_ * y_m1;
	y_m1 = y_;
	x_m1 = x_;
			
	return y_;
}
