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
    CrossOver(float frequency);
    
    ~CrossOver() {}
   
    void setup(float frequency);
    
    void setFrequency(float frequency);
    
    void process(float value, float frequency);
    
    float getLow();
    float getHigh();
    
private:
    float w_c;
    float a_;
    float b_0;
    float x_n_m1;
    float y_n_m1;
    float y_l;
    float y_h;
};

#endif /* CrossOver_h */
