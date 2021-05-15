//
//  Spectral_Balance.cpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 03.01.21.
//  Copyright © 2021 JUCE. All rights reserved.
//

#include "Spectral_Balance.h"

Spectral_Balance::Spectral_Balance(float filterFrequency, float rmsFreq, float sampleRate){
    initialise(440.0, 10.0, 48000.0);
}

void Spectral_Balance::initialise(float filterFrequency, float rmsFreq, float sampleRate){
    filterFreq_ = filterFrequency;
    rmsFreq_ = rmsFreq;
    sampleRate_ = sampleRate;
    inverseSampleRate_ = 1.0 / sampleRate_;
    nyquist_ = sampleRate_ / 2.0;
    crossOver.setup(filterFreq_, sampleRate_);
    rmsLow.setup(rmsFreq_, sampleRate_);
    rmsHigh.setup(rmsFreq_, sampleRate_);
    rmsDiffDivide.setup(rmsFreq_, sampleRate_);
    integrator.setup(rmsFreq_, sampleRate_);
    xOverFreq_ = 440.0;
}

void Spectral_Balance::setFrequency(float filterFrequency){
    filterFreq_ = filterFrequency;
}

void Spectral_Balance::setRMSFreq(float rmsFreq){
    rmsFreq_ = rmsFreq;
}

void Spectral_Balance::process(float value){
    
    crossOver.process(value, integrator_);
    
    float rms = rmsDiffDivide.process(value);
    
    float xOverLow = crossOver.getLow();
    float xOverHigh = crossOver.getHigh();
    
    xOverLow = rmsLow.process(xOverLow);
    xOverHigh = rmsHigh.process(xOverHigh);
    
    float difference = xOverHigh - xOverLow;
    
    difference /=  rms;
    
    difference *= rmsFreq_;
    difference *= inverseSampleRate_;
    
    integrator_ = integrator.process(difference);
    
    integrator_ = pow(integrator_, 4.0);
    
    integrator_ = clamp2(integrator_, 0.0f, 1.0f);
    
    integrator_ = log10(integrator_ * 9.0f + 1.0f);
}

float Spectral_Balance::getTendencyHZ(){
    xOverFreq_ = integrator_ * nyquist_;
    return xOverFreq_;
}
