/***** Delay.cpp *****/


#include "Delay.h"
#include <cmath>

Delay::Delay(float maxDelayTime, float delayTime, float feedBack, float sampleRate) {
	setup(maxDelayTime = 1.0, delayTime = 0.2, feedBack = 0.0, sampleRate = 48000);
}

void Delay::setup(float maxDelayTime, float delayTime, float feedBack, float sampleRate)
{
    delayTime_ = delayTime * sampleRate;
	maxDelayTime_ = maxDelayTime * sampleRate;
    buffer_.setup(maxDelayTime_, delayTime_);
	feedBack_ = feedBack;
	sampleRate_ = sampleRate;
}

// Set delayTime
void Delay::setDelayTime(float delayTime) {
delayTime_ = delayTime * sampleRate_;
}

// Set feedback
void Delay::setFeedback(float feedBack) {
feedBack_ = feedBack;
}

// Filter function
float Delay::process(float in) {
	
    float delay = buffer_.read();
    buffer_.write(in + (delay * feedBack_));
			
	return delay;
}
