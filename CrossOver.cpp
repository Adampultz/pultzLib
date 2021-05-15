//
//  CrossOver.cpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 26.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//

#include "CrossOver.h"

CrossOver::CrossOver(float frequency, float sampleRate){
    setup(frequency = 440.0, sampleRate = 48000.0);
    inverseSampleRate_ = 1.0 / sampleRate;
}

void CrossOver::setup(float frequency, float sampleRate){
    inverseSampleRate_ = 1.0 / sampleRate;
    }
    
void CrossOver::process(float value, double frequency){  // TODO: return array of high and low values
    float x_ = value;
    w_c = 2.0f * M_PI *  frequency;
    a_ = (1.0f - sin(w_c)) / cos(w_c);
    b_0 = (1.0f - a_) * 0.5f; // b_1 is identical to b_0
    y_l = (b_0 * x_) + (b_0 * x_n_m1) + (a_ * y_n_m1); // Low pass
    y_h = x_ - y_l; // High pass, optimised filter section
    y_n_m1 = y_l;
    x_n_m1 = x_;
    }

float CrossOver::getLow(){
    return y_l;
}

float CrossOver::getHigh(){
    return y_h;
}
