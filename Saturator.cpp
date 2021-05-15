/***** Saturator.cpp *****/

#include "Saturator.h"

Saturator::Saturator(float coeff, float bal){
	init(coeff, bal);
}

void Saturator::init(float coeff, float bal){
	coeff_ = coeff;
	bal_ = bal;
	leakyInt.init(coeff_);
	balance.init(bal_);
}

float Saturator::process(float val, float coeff, float amp){
	leakyInt.setCoeff(coeff);
	balance.setBalance(amp);
	float x_ = fabs(val) * 8.0f;
	float filter = leakyInt.process(x_);
	filter = val * (1.0f / filter);
	float output = balance.process(filter, val);
	
	return output;
}
