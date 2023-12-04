/***** RMS.h *****/

#pragma once

#include "Definitions.hpp"
#include <stdio.h>
#include <cmath>
#include "Utilities.hpp"


class RMS : public Definitions {
	
public:

	RMS() {}
    // Default constructor
	RMS(float frequency);
			  
	void init(float frequency = 10); 		
	
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
};
