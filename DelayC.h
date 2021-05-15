/***** Delay.h *****/

// Delay line with feedback, linear interpolation and sample-by-sample smoothing ramp

#pragma once
#include "CircularBufferC.h"

using namespace pultzLib;

class DelayC {
public:
	DelayC() {}													// Default constructor
	DelayC(float maxDelayTime, float delayTime, float feedBack, float sampleRate);
			  
	void setup(float maxDelayTime, float delayTime, float feedBack, float sampleRate); 		
	
    void setDelay(int delay);
	void setDelayTime(float delayTime);
	void setFeedback(float feedBack);
	
	float process(float in);				// Get the next sample and update the phase
	
	~DelayC() {}				// Destructor

private:
	
    CircularBufferC<float> buffer_;
	int maxDelayTime_;  // Maximum delay time in samples
	int delayTime_;
	float sampleRate_;           // Samplerate
	float feedBack_; // Filter difference equation (x)
	
};
