//  Spectral_Balance.cpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//  Created by Adam Pultz Melbye on 03.01.21.


#include "Spectral_Balance_Cheap.h"

Spectral_Balance_Cheap::Spectral_Balance_Cheap(float filterFrequency, float rmsFreq, float sampleRate){
    initialise(0.1, 10.0, 48000.0);
}

void Spectral_Balance_Cheap::initialise(float filterFrequency, float rmsFreq, float sampleRate){
    filterFreq_ = filterFrequency;
    rmsFreq_ = rmsFreq;
    sampleRate_ = sampleRate;
    inverseSampleRate_ = 1.0f / sampleRate_;
    response_ = rmsFreq * inverseSampleRate_;
    nyquist_ = sampleRate_ / 2.0f;
    nyquist_Adjusted = nyquist_ * 0.66333f; // Adjust Nyquist by scaling factor
    crossOver.setup(filterFreq_, sampleRate_);
    rmsLow.setup(rmsFreq_, sampleRate_);
    rmsHigh.setup(rmsFreq_, sampleRate_);
    rmsDiffDivide.setup(rmsFreq_, sampleRate_);
    integrator.setup(rmsFreq_, sampleRate_);
    xOverFreq_ = 0.1f;
}

void Spectral_Balance_Cheap::setFrequency(float filterFrequency){
    filterFreq_ = filterFrequency;
}

void Spectral_Balance_Cheap::setRMSFreq(float rmsFreq){
    rmsFreq_ = rmsFreq;
    response_ = rmsFreq * inverseSampleRate_;
 }

void Spectral_Balance_Cheap::process(float value){
    
    crossOver.process(value, integrator_);
    
    float rms = clamp2(rmsDiffDivide.process(value), 0.0001f, 1.0f);
    
    xOverLow = crossOver.getLow();
    xOverHigh = crossOver.getHigh();
    
    xOverLow = rmsLow.process(xOverLow);
    xOverHigh = rmsHigh.process(xOverHigh);
    
    difference = xOverHigh - xOverLow;
    
    difference /=  rms;
    
    difference *= response_;
    
    integrator_ = integrator.process(difference);
    
    integrator_ = clamp2(integrator_, 0.0f, 1.0f);
    
    integrator_ = powf(integrator_, 2.0f);
}

float Spectral_Balance_Cheap::getTendencyHZ(){
    xOverFreq_ = integrator_ * nyquist_Adjusted;
    return xOverFreq_;
}
