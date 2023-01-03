/*
 A C++ implementation of Dario Sanfilippo's "Spectral Tendency" algorithm. See Sanfilippo's PhD thesis "Complex Musical Behaviours via Time-Variant Audio Feedback Networks and Distributed Adaptation: a Study of Autopoietic Infrastructures for Real-Time Performance Systems" (2019)
 */
#pragma once

#ifndef Spectral_Balance_hpp
#define Spectral_Balance_hpp

#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include "RMS.h"
#include "CrossOver_Cheap.h"
#include "Integrator.h"
#include "fundamentals.hpp"
#include "Fast_Math.hpp"

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
    float integrator_;
    float xOverLow;
    float xOverHigh;
    float difference;
    
    RMS rmsLow;
    RMS rmsHigh;
    RMS rmsDiffDivide;
    CrossOver_Cheap crossOver;
    Integrator integrator;
};

#endif /* Spectral_Balance_hpp */
