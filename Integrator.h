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
    
   double process(float in);
    
private:
    double y_n_m1 = 0.0;
    float rate_;
    float sampleRate_;
    double inverseSampleRate_;
};

#endif /* Integrator_h */
