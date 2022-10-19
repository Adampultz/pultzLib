/***** Dc.h *****/

#pragma once

class DCBlock {
public:
	DCBlock() {}													// Default constructor
	DCBlock(float freq, float sampleRate);
			  
	void setup(float freq, float sampleRate); 		
	
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
	float inverseSampleRate_;
};