/***** Dc.h *****/

#pragma once

class DCBlock {
public:
	DCBlock() {}													// Default constructor
	DCBlock(float freq, float sampleRate);
			  
	void setup(float freq, float sampleRate); 		
	
	void setFrequency(float freq);
	
	float process(float in);
	
	~DCBlock() {}

private:
	
	float coeff_;
	float x_;
	float y_;
	float x_m1;			
	float y_m1;
	float frequency_;
	float inverseSampleRate_;
};
