//
//  Delay.hpp
//  Basic_DSP - App
//
//  Created by Adam Pultz Melbye on 18/11/2023.
//

#ifndef Delay_hpp
#define Delay_hpp

#pragma once
#include "Definitions.hpp"
#include "CircularBuffers.h"

namespace pultzLib {

class Delay {
public:
    Delay() {}
    Delay(float maxDelayTime, float delayTime, float feedBack);
              
    void init(float maxDelayTime, float delayTime);
    
    void setDelaySamp(int delay);
    void setDelayTime(float delayTime);
    void setFeedback(float feedBack);
    
    float process(float in);
    
    ~Delay() {}

private:
    
    CircularBuffer<float> buffer_;
    int maxDelayTime_;  // Maximum delay time in samples
    int delayTime_;
};

}

#endif /* Delay_hpp */
