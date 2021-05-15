//
//  LinLin.cpp
//  LoopingAudioSampleBufferAdvancedTutorial
//
//  Created by Adam Pultz Melbye on 15.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//

#include "LinLin.h"

LinLin::LinLin(float inMin, float inMax, float outMin, float outMax){
    setup(inMin, inMax, outMin, outMax);
}

    
void LinLin::setup(float inMin, float inMax, float outMin, float outMax){
        a_ = (outMax - outMin) / (inMax - inMin);
        b_ = outMax - a_ * inMax;
    outMin_ = outMin;
    outMax_ = outMax;
    }
    
float LinLin::process(float value){
        float newVal = a_ * value + b_;
        return newVal;
    }

