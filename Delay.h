/***** Delay.h *****/

// Delay line with feedback, linear interpolation and sample-by-sample smoothing ramp

#pragma once
#include "CircularBuffer.h"

using namespace pultzLib;

class Delay {
public:
	Delay() {}
	Delay(float maxDelayTime, float delayTime, float feedBack, float sampleRate);
			  
	void setup(float maxDelayTime, float delayTime, float feedBack, float sampleRate); 		
	
    void setDelay(int delay);
	void setDelayTime(float delayTime);
	void setFeedback(float feedBack);
	
	float process(float in);
	
	~Delay() {}

private:
	
    CircularBuffer<float> buffer_;
	int maxDelayTime_;  // Maximum delay time in samples
	int delayTime_;
	float sampleRate_;
	float feedBack_; 
	
};
