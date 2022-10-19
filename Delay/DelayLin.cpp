/***** Delay.cpp *****/


#include "DelayLin.h"
#include <cmath>

// Constructor taking arguments for sample rate and table data
DelayLin::DelayLin(float maxDelayTime, float delayTime, float feedBack, float sampleRate) {
	setup(maxDelayTime = 1.0, delayTime = 0.2, feedBack = 0.0, sampleRate = 48000);
}

void DelayLin::setup(float maxDelayTime, float delayTime, float feedBack, float sampleRate)
{
    delayTime_ = delayTime * sampleRate;
	maxDelayTime_ = maxDelayTime * sampleRate;
    buffer_.setup(maxDelayTime_, delayTime_);
	feedBack_ = feedBack;
	sampleRate_ = sampleRate;
}

// Set delayTime
void DelayLin::setDelayTime(float delayTime) {
delayTime_ = delayTime * sampleRate_;
    buffer_.setReadIndex(delayTime_);
}

// Set feedback
void DelayLin::setFeedback(float feedBack) {
feedBack_ = feedBack;
}

// Filter function
float DelayLin::process(float in) {
	
    float delay = buffer_.read();
    buffer_.write(in + (delay * feedBack_));
			
	return delay;
}

