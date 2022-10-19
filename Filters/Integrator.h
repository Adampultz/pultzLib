//
//  Integrator.hpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 26.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//

#ifndef Integrator_h
#define Integrator_h

#include <stdio.h>
#include <cmath>

class Integrator
{
public:
    Integrator(){}
    Integrator(float rate, float sampleRate);
    
    void setup(float rate, float sampleRate);
    
    ~Integrator() {}
    
   float process(float val);
    
private:
    float y_n_m1;
    float rate_;
    float sampleRate_;
    float inverseSampleRate_;
};

#endif /* Integrator_h */
