/***** EnvelopeFollower.h *****/
#pragma once

#include <cmath>

class EnvelopeFollower {
public:
	EnvelopeFollower();												// Default constructor
	EnvelopeFollower(float sampleRate);
	
	void setSampleRate(float sampleRate);
			  
	void setup(float attack, float decay, float sampleRate); 		
	
	void setAttack(float attack);
	void setDecay(float decay);
    
    void calculateCoefficients(float attack, float decay);
	
	float process(float input);				// Get the next sample and update the phase
	
	~EnvelopeFollower() {}				// Destructor

private:
	float attack_;           // Input of the filter
	float decay_;			// Filter difference equation (x)
	float lastOutput_;
	float sampleRate_;
	float inverseSampleRate_;
	float gEnvAtt_;
	float gEnvDecay_;
};
