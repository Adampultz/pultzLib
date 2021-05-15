/***** EnvelopeFollower.cpp *****/
#include "EnvelopeFollower.h"

// Constructor taking arguments for sample rate and table data
EnvelopeFollower::EnvelopeFollower() {}

void EnvelopeFollower::setup(float attack, float decay, float sampleRate)
{
	sampleRate_ = sampleRate;
	inverseSampleRate_ = 1 / sampleRate;
	gEnvAtt_ = pow(exp(-1 / attack), inverseSampleRate_);
	gEnvDecay_ = pow(exp(-1 / decay), inverseSampleRate_);
	
}


void EnvelopeFollower::setAttack(float attack) {
attack_ = attack;
calculateCoefficients(attack_, decay_);
}

void EnvelopeFollower::setDecay(float decay) {
decay_ = decay;
calculateCoefficients(attack_, decay_);
}

void EnvelopeFollower::calculateCoefficients(float attack, float decay)
{
	gEnvAtt_ = pow(exp(-1 / attack_), inverseSampleRate_);
gEnvDecay_ =  pow(exp(-1 / decay_), inverseSampleRate_);
}

// Filter function
float EnvelopeFollower::process(float input) {
	float inAbs_ = fabs(input);
	float out = 0;
			
	if (inAbs_ > lastOutput_) {
	out = gEnvAtt_ * lastOutput_ + ((1.0f - gEnvAtt_) * inAbs_);
	}
	else{
	out = gEnvDecay_ * lastOutput_ + ((1.0f - gEnvDecay_) * inAbs_);
	}
			
	lastOutput_ = out;

	return out;
}
