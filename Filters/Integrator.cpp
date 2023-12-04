//
//  Integrator.cpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 26.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//

#include "Integrator.h"

Integrator::Integrator(float rate){
    init(rate = 15.0);
}

void Integrator::init(float rate){
    rate_ =  1.0f / rate;
}

float Integrator::process(float val){
    float y = (val *  rate_)  + y_n_m1;
    y_n_m1 = y;
    
    return y;
}
