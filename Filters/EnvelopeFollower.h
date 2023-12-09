/***** EnvelopeFollower.h *****/
#pragma once

#include "Definitions.hpp"
#include "Utilities.hpp"

class EnvelopeFollower : public Definitions {
public:
	EnvelopeFollower();												// Default constructor
			  
	void init(float attack, float decay); 		
	
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
	float gEnvAtt_;
	float gEnvDecay_;
	float statEnvAtt;
	float statEnvDec;
};
