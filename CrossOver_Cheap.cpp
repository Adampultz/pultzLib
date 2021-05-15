//
//  CrossOver.cpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 26.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//

#include "CrossOver_Cheap.h"

CrossOver_Cheap::CrossOver_Cheap(float frequency, float sampleRate){
    setup(frequency = 0.1, sampleRate = 48000.0);
    inverseSampleRate_ = 1.0 / sampleRate;
}

void CrossOver_Cheap::setup(float frequency, float sampleRate){
    inverseSampleRate_ = 1.0 / sampleRate;
    }
    
void CrossOver_Cheap::process(float value, double frequency){  // TODO: return array of high and low values
    float x_ = value;
    a_ = frequency;
    b_1 = 1.0f - (2.0 * frequency);
    y_l = ((a_ * x_) + (a_ * x_n_m1) + (b_1 * y_n_m1)); // Low pass
    y_h = x_ - y_l; // High pass, optimised filter section
    y_n_m1 = y_l;
    x_n_m1 = x_; 
    }

float CrossOver_Cheap::getLow(){
    return y_l;
}

float CrossOver_Cheap::getHigh(){
    return y_h;
}


