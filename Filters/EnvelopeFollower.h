/***** EnvelopeFollower.h *****/
#pragma once

#include "Utilities.hpp"

class EnvelopeFollower {
public:
	EnvelopeFollower();												// Default constructor
	EnvelopeFollower(float sampleRate);
	
	void setSampleRate(float sampleRate);
			  
	void setup(float attack, float decay, float sampleRate); 		
	
	void setAttack(float attack);
	void setDecay(float decay);
	
	float getdb();
	
	float process(float input);				// Get the next sample and update the phase
	
	~EnvelopeFollower() {}				// Destructor

private:
	void calculateCoefficients(float frequency, float q);
	float attack_;           // Input of the filter
	float decay_;			// Filter difference equation (x)
	float inAbs_;
	float lastOutput_;
	double sampleRate_;
	double inverseSampleRate_;
	float gEnvAtt_;
	float gEnvDecay_;
	float statEnvAtt;
	float statEnvDec;
};