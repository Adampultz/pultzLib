//
//  Median.cpp
//  LoopingAudioSampleBufferAdvancedTutorial
//
//  Created by Adam Pultz Melbye on 17.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//


#include "Median.h"

//Median::Median(){}

float Median::process(float in){
      
    float median = std::min(std::max(in, b_), std::max(b_, c_));
    
    b_ = in;
    c_ = b_;
    
    return median;
    }


