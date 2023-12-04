/***** Delay.cpp *****/


#include "DelayLin.h"
#include <cmath>

using namespace pultzLib;

DelayLin::DelayLin(float maxDelayTime, float delayTime, float feedBack) {
	init(maxDelayTime = 1.0, delayTime = 0.2, feedBack = 0.0);
}

void DelayLin::init(float maxDelayTime, float delayTime, float feedBack) {
    delayTime_ = delayTime * g_SampleRate;
	maxDelayTime_ = maxDelayTime * g_SampleRate;
    buffer_.init(maxDelayTime_, delayTime_);
	feedBack_ = feedBack;
}

void DelayLin::setDelayTime(float delayTime) {
    delayTime_ = delayTime * g_SampleRate;
    buffer_.setReadIndex(delayTime_);
}

void DelayLin::setFeedback(float feedBack) {
    feedBack_ = feedBack;
}

float DelayLin::process(float in) {
	
    float delay = buffer_.read();
    buffer_.writePow2(in + (delay * feedBack_));
			
	return delay;
}

