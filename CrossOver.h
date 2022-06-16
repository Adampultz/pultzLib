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
    
    void process(float value, float frequency);
    
    float getLow();
    float getHigh();
    
private:
    float sampleRate_;
    float inverseSampleRate_;
    float w_c;
    float a_;
    float b_0;
    float x_n_m1;
    float y_n_m1;
    float y_l;
    float y_h;
};

#endif /* CrossOver_h */
