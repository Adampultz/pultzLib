//  Crossover using one-pole one-zero low and high pass filters in parallel. See Darion SanFillipo 

#pragma once

#ifndef CrossOver_Cheap_h
#define CrossOver_Cheap_h

#include <stdio.h>
#include <cmath>
#include <vector>

class CrossOver_Cheap
{
public:
    CrossOver_Cheap(){}
    CrossOver_Cheap(float frequency, float sampleRate);
    
    ~CrossOver_Cheap() {}
   
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
   double b_0;
    double b_1;
    double x_n_m1;
    double y_n_m1;
    double y_l;
    double y_h;
};

#endif /* CrossOver_Cheap_h */
