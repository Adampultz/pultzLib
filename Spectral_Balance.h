//
//  Spectral_Balance.hpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 03.01.21.
//  Copyright © 2021 JUCE. All rights reserved.
//
#pragma once

#ifndef Spectral_Balance_hpp
#define Spectral_Balance_hpp

#include <stdio.h>
#include "RMS.h"
#include <cstdint>
#include <algorithm>
#include "CrossOver.h"
#include "Integrator.h"
#include "fundamentals.hpp"

class Spectral_Balance
{
public:
    Spectral_Balance(){}
    Spectral_Balance(float filterFrequency, float rmsFreq, float sampleRate);
    
    ~Spectral_Balance() {}
   
    void initialise(float filterFrequency, float rmsFreq, float sampleRate);
    
    void setFrequency(float filterFrequency);
    
    void setRMSFreq(float rmsFreq);
    
    void process(float value);
    
    float getTendencyHZ();
    
private:

    float filterFreq_;
    float rmsFreq_;
    float sampleRate_;
    float inverseSampleRate_;
    float nyquist_;
    float xOverFreq_;
    float integrator_;
    
    RMS rmsLow;
    RMS rmsHigh;
    RMS rmsDiffDivide;
    CrossOver crossOver;
    Integrator integrator;
};

#endif /* Spectral_Balance_hpp */
