/***** EnvelopeFollower.cpp *****/
#include "EnvelopeFollower.h"
#include <cmath>


// Constructor taking arguments for sample rate and table data
EnvelopeFollower::EnvelopeFollower() {}

void EnvelopeFollower::init(float attack, float decay)
{
    gEnvAtt_ = powf(exp(-1.0 / attack), g_SampleDur);
    gEnvDecay_ = powf(exp(-1.0 / decay), g_SampleDur);
	statEnvAtt = 1.0 - gEnvAtt_;
	statEnvDec = 1.0 - gEnvDecay_;
}


void EnvelopeFollower::setAttack(float attack) {
attack_ = attack;
calculateCoefficients(attack_, decay_);
}

void EnvelopeFollower::setDecay(float decay) {
decay_ = decay;
calculateCoefficients(attack_, decay_);
}

float EnvelopeFollower::getdb(){
	float dB = a2db(lastOutput_);
	return dB;
}

void EnvelopeFollower::calculateCoefficients(float attack, float decay)
{
    gEnvAtt_ = pow(exp(-1.0 / attack_), g_SampleDur);
    gEnvDecay_ =  pow(exp(-1.0 / decay_), g_SampleDur);
}

// Filter function
float EnvelopeFollower::process(float input) {
	inAbs_ = fabs(input);
	float out = 0.0f;
			
	if (inAbs_ > lastOutput_) {
	out = gEnvAtt_ * lastOutput_ + (statEnvAtt * inAbs_);
	}
	else{
	out = gEnvDecay_ * lastOutput_ + (statEnvDec * inAbs_);
	}
			
	lastOutput_ = out;

	return out;
}
