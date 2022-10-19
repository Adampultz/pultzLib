/***** RMS.cpp *****/

#include "RMS.h"


// Constructor taking arguments for sample rate and table data
RMS::RMS(float frequency, float sRate) {
	setup(frequency, sRate);
}

void RMS::setup(float frequency, float sRate)
{
	ym1_ = 0.0f;
	frequency_ = frequency;
	sRate_ = sRate;
	inverseSampleRate_ = 1.0f / sRate;
	b1_ = expf(-2.0f * (float)M_PI * (frequency_ * inverseSampleRate_));
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
    frequency_ = frequency;
	b1_ = expf(-2.0f * (float)M_PI * (frequency_ * inverseSampleRate_));
    a0_ = 1.0f - b1_;
}

// Filter function
float RMS::process(float input) {
    ym1_ = sqrtf(powf(input, 2.0f) * a0_ + powf(ym1_, 2.0f) * b1_);

    return ym1_;
}
