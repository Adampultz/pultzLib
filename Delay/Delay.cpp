//
//  Delay.cpp
//  Basic_DSP - App
//
//  Created by Adam Pultz Melbye on 18/11/2023.
//

#include "Delay.hpp"
#include <cmath>

using namespace pultzLib;

Delay::Delay(float maxDelayTime, float delayTime, float feedBack) {
    init(maxDelayTime = 1.0, delayTime = 0.2);
}

void Delay::init(float maxDelayTime, float delayTime){
    delayTime_ = delayTime * g_SampleRate;
    maxDelayTime_ = maxDelayTime * g_SampleRate;
    buffer_.init(maxDelayTime_, delayTime_);
}

void Delay::setDelayTime(float delayTime) {
    delayTime_ = delayTime * g_SampleRate;
    buffer_.setReadIndex(delayTime_);
}

void Delay::setDelaySamp(int delay) {
    buffer_.setReadIndex(delay);
}

float Delay::process(float in) {
    float delay = buffer_.readPow2();
    buffer_.writePow2(in);
            
    return delay;
}

