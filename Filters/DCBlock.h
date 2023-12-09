/***** Dc.h *****/

#pragma once
#include "Definitions.hpp"
#include <cmath>


class DCBlock {
public:
	DCBlock() {}													// Default constructor
	DCBlock(float freq);
			  
	void init(float freq);
	
	void setFrequency(float freq);
	
	float process(float in);				// Get the next sample and update the phase
	
	~DCBlock() {}				// Destructor

private:
	
	float coeff_;           // Frequency of the filter
	float x_;           // Input of the filter
	float y_;
	float x_m1;			// Filter difference equation (x)
	float y_m1;			 // output of filter (y-1)
	float frequency_;
};
