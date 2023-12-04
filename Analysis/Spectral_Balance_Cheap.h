/*
 A C++ implementation of Dario Sanfilippo's "Spectral Tendency" algorithm. See Sanfilippo's PhD thesis "Complex Musical Behaviours via Time-Variant Audio Feedback Networks and Distributed Adaptation: a Study of Autopoietic Infrastructures for Real-Time Performance Systems" (2019)
 */
#pragma once

#ifndef Spectral_Balance_hpp
#define Spectral_Balance_hpp

#include "Definitions.hpp"
#include <stdio.h>
#include <cstdint>
#include <algorithm>
#include "RMS.h"
#include "CrossOver_Cheap.h"
#include "Integrator.h"
#include "fundamentals.hpp"
#include "Fast_Math.hpp"

class Spectral_Balance_Cheap : public Definitions {
public:
    Spectral_Balance_Cheap(){}
    Spectral_Balance_Cheap(float filterFrequency, float rmsFreq);
    
    ~Spectral_Balance_Cheap() {}
   
    void init(float filterFrequency, float rmsFreq);
    
    void setFrequency(float filterFrequency);
    
    void setRMSFreq(float rmsFreq);
    
    void process(float value);
    
    float getTendencyHZ();
    
private:

    float filterFreq_;
    float rmsFreq_;
    float response_;
    float nyquist_Adjusted_;
    float xOverFreq_;
    float integrator_;
    
    RMS rmsLow;
    RMS rmsHigh;
    RMS rmsDiffDivide;
    CrossOver_Cheap crossOver;
    Integrator integrator;
};

#endif /* Spectral_Balance_hpp */
