/***** Saturator.cpp *****/

#include "Saturator.h"

Saturator::Saturator(float coeff){
	init(coeff);
}

void Saturator::init(float coeff){
	coeff_ = coeff;
	leakyInt.init(coeff_);
}

float Saturator::process(float val, float coeff, float amp){
	leakyInt.setCoeff(coeff);
	float x_ = fabsf_neon(val);
	float filter = leakyInt.process(x_);
	filter = val * (1.0f / filter);
	
	return Balance2(filter, val, clamp2(amp, 0.0f, 1.0f));
}
