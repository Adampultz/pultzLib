//
//  CrossOver.hpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 26.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//  Crossover using one-pole one-zero low and high pass filters in parallel. See Darion SanFillipo 

#pragma once

#ifndef CrossOver_h
#define CrossOver_h

#include <stdio.h>
#include <cmath>
#include <vector>

class CrossOver
{
public:
    CrossOver(){}
    CrossOver(float frequency, float sampleRate);
    
    ~CrossOver() {}
   
    void setup(float frequency, float sampleRate);
    
    void setFrequency(float frequency);
    
    void process(float value, double frequency);
    
    float getLow();
    float getHigh();
    
private:
    float sampleRate_;
    double inverseSampleRate_;
    float w_c;
    float a_;
    float b_0;
    float x_n_m1;
    float y_n_m1;
    float y_l;
    float y_h;
};

#endif /* CrossOver_h */
