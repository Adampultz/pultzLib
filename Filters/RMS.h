/***** RMS.h *****/

#pragma once

#include <stdio.h>
#include <cmath>
#include "Utilities.hpp"


class RMS {
	
public:

	RMS() {}													// Default constructor
	RMS(float frequency, float sRate);
			  
	void setup(float frequency = 10, float sRate = 48000); 		
	
	void setFrequency(float frequency);
	
	float getDb();
	
	float getDbNorm();
	
	float process(float input);				// Get the next sample and update the phase
	
	~RMS() {}				// Destructor

private:
	
	float a0_;           // Input of the filter
	float b1_;
	float ym1_;
	float frequency_;
	float sRate_;
	float inverseSampleRate_;
};