//
//  Compressor.cpp
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 24.03.21.
//

#include "Compressor.hpp"


// Constructor taking arguments for sample rate and table data
Compressor::Compressor() {}

void Compressor::initialise(float threshold, float ratio, float attack, float decay, float sampleRate, float makeupGain)
{
    sampleRate_ = sampleRate;
    inverseSampleRate_ = 1.0 / sampleRate;
    alphaAtt_ = exp( - 1.0 / (0.001 * sampleRate_ * attack));
    alphaRelease_ = exp( - 1.0 / (0.001 * sampleRate_ * decay));
    threshold_ = threshold;
    ratio_ = ratio;
    ratioRecip_ = 1 / ratio_;
    makeUpGain_ = makeupGain;
}


void Compressor::setAttack(float attack) {
attack_ = attack;
}

void Compressor::setDecay(float decay) {
decay_ = decay;
}

// Filter function
float Compressor::process(float input) {
    float s_ = input;
    float s_abs_ = fabs(input);
    
    if (fabs(s_) < 0.000001)
        x_g = -120.0f;
    else
        x_g = a2db(s_abs_);
    
    if (x_g >= threshold_)
        y_g = threshold_ + (x_g - threshold_) * ratioRecip_;
        else
        y_g = x_g;
    
    x_l = x_g - y_g;
    
    // Ballistics - smoothing of the gain
    
    if (x_l > y_l_prev)
        y_l = alphaAtt_ * y_l_prev + (1 - alphaAtt_) * x_l;
    else
        y_l = alphaRelease_ * y_l_prev + (1 - alphaRelease_) * x_l;
    
    c_ = db2a(makeUpGain_ - y_l);
    
    y_l_prev = y_l;

    return s_ * c_;
}

