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
#include "CrossOver_Cheap.h"
#include "Integrator.h"
#include "fundamentals.hpp"

class Spectral_Balance_Cheap
{
public:
    Spectral_Balance_Cheap(){}
    Spectral_Balance_Cheap(float filterFrequency, float rmsFreq, float sampleRate);
    
    ~Spectral_Balance_Cheap() {}
   
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
    float response_;
    float nyquist_;
    float nyquist_Adjusted;
    float xOverFreq_;
    double integrator_;
    
    RMS rmsLow;
    RMS rmsHigh;
    RMS rmsDiffDivide;
    CrossOver_Cheap crossOver;
    Integrator integrator;
};

#endif /* Spectral_Balance_hpp */
