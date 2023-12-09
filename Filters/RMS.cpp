/***** RMS.cpp *****/

#include "RMS.h"


// Constructor taking arguments for sample rate and table data
RMS::RMS(float frequency) {}

void RMS::init(float frequency)
{
	ym1_ = 0.0f;
	b1_ = expf(-2.0f * (float)M_PI * (frequency * g_SampleDur));
	a0_ = 1.0f - b1_;
}

float RMS::getDb() // Only call after process
{
	return a2db(ym1_);
}

float RMS::getDbNorm()
{
	float db = this->getDb();
	return mapLin(db, -60.0, 0, 0, 1.0);
}

// Set the frequency 
void RMS::setFrequency(float frequency) {
	b1_ = expf(-2.0f * (float)M_PI * (frequency * g_SampleDur));
    a0_ = 1.0f - b1_;
}

// Filter function
float RMS::process(float input) {
    ym1_ = sqrtf(powf(input, 2.0f) * a0_ + powf(ym1_, 2.0f) * b1_);

    return ym1_;
}
