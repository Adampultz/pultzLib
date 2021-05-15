
#pragma once

#ifndef Compressor_hpp
#define Compressor_hpp

#include <stdio.h>
#include <math.h>
#include "db_lookup_table.hpp"

using namespace pultzLib;

class Compressor {
    
public:
    Compressor();                                                // Default constructor
    Compressor(float sampleRate);
    
    void setSampleRate(float sampleRate);
              
    void initialise(float threshold, float ratio, float attack, float decay, float sampleRate, float makeupGain);
    
    void setAttack(float attack);
    void setDecay(float decay);
    float process(float input);                // Get the next sample and update the phase
    
    ~Compressor() {}                // Destructor

private:
//    EnvelopeFollower envelope;

    float attack_;           // Input of the filter
    float decay_;            // Filter difference equation (x)
    float lastOutput_;
    float sampleRate_;
    float inverseSampleRate_;
    float alphaAtt_;
    float alphaRelease_;
    float threshold_;
    float ratio_;
    float ratioRecip_;
    float x_g, x_l, y_g, y_l;
    float y_l_prev;
    float makeUpGain_;
    float c_; 
    
};


#endif /* Compressor_hpp */
