/***** Delay.h *****/

/*
 Delay line with feedback, linear interpolation
 */

#pragma once
#include "Definitions.hpp"
#include "CircularBuffers.h"

namespace pultzLib {


class DelayLin : public Definitions {
public:
	DelayLin() {}				
	DelayLin(float maxDelayTime, float delayTime, float feedBack);
			  
	void init(float maxDelayTime, float delayTime, float feedBack);
	
    void setDelay(int delay);
	void setDelayTime(float delayTime);
    void setDelaySamp(float delayTime);
	void setFeedback(float feedBack);
	
	float process(float in);
	
	~DelayLin() {}

private:
	
    CircularBufferLin<float> buffer_;
	int maxDelayTime_;  // Maximum delay time in samples
	int delayTime_;
	float feedBack_;
};

}
