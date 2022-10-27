/***** Delay.h *****/

/*
 Delay line with feedback, linear interpolation
 */

#pragma once
#include "CircularBuffers.h"

namespace pultzLib {


class DelayLin {
public:
	DelayLin() {}				
	DelayLin(float maxDelayTime, float delayTime, float feedBack, float sampleRate);
			  
	void init(float maxDelayTime, float delayTime, float feedBack, float sampleRate);
	
    void setDelay(int delay);
	void setDelayTime(float delayTime);
	void setFeedback(float feedBack);
	
	float process(float in);
	
	~DelayLin() {}

private:
	
    CircularBufferLin<float> buffer_;
	int maxDelayTime_;  // Maximum delay time in samples
	int delayTime_;
	float sampleRate_;
	float feedBack_; 
	
};

}
