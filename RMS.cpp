/***** RMS.cpp *****/

#include "RMS.h"
#include <stdio.h>
#include <cmath>

// Constructor taking arguments for sample rate and table data
RMS::RMS(float frequency, float sRate) {
	setup(frequency, sRate);
}

void RMS::setup(float frequency, float sRate)
{
	ym1_ = 0.0;
	frequency_ = frequency;
	sRate_ = sRate;
	inverseSampleRate_ = 1.0f / sRate;
	b1_ = expf(-2.0f * (float)M_PI * (frequency_ * inverseSampleRate_));
	a0_ = 1.0f - b1_;
}

// Set the frequency 
void RMS::setFrequency(float frequency) {
    frequency_ = frequency;
	b1_ = expf(-2.0f * (float)M_PI * (frequency_ * inverseSampleRate_));
    a0_ = 1.0f - b1_;
}

// Filter function
float RMS::process(float input) {
    ym1_ = sqrtf(powf(input, 2.0) * a0_ + powf(ym1_, 2.0) * b1_);

    return ym1_;
}
