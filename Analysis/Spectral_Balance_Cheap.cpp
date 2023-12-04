#include "Spectral_Balance_Cheap.h"

Spectral_Balance_Cheap::Spectral_Balance_Cheap(float filterFrequency, float rmsFreq){
    init(0.1, 10.0);
}

void Spectral_Balance_Cheap::init(float filterFrequency, float rmsFreq){
    filterFreq_ = filterFrequency;
    rmsFreq_ = rmsFreq;
    response_ = rmsFreq * g_SampleDur;
    nyquist_Adjusted_ = g_Nyquist * 0.66333f; // Adjust Nyquist by scaling factor
    crossOver.init(filterFreq_);
    rmsLow.init(rmsFreq_);
    rmsHigh.init(rmsFreq_);
    rmsDiffDivide.init(rmsFreq_);
    integrator.init(rmsFreq_);
    xOverFreq_ = 0.1f;
}

void Spectral_Balance_Cheap::setFrequency(float filterFrequency){
    filterFreq_ = filterFrequency;
}

void Spectral_Balance_Cheap::setRMSFreq(float rmsFreq){
    rmsFreq_ = rmsFreq;
    response_ = rmsFreq * g_SampleDur;
 }

void Spectral_Balance_Cheap::process(float value){
    
    crossOver.process(value, integrator_);
    
    float rms = clamp2(rmsDiffDivide.process(value), 0.0001f, 1.0f); // Constrain rms
    
    float xOverLow = crossOver.getLow(); // ToDo: make crossover spit out tuple of low and high
    float xOverHigh = crossOver.getHigh();
    
    xOverLow = rmsLow.process(xOverLow);
    xOverHigh = rmsHigh.process(xOverHigh);
    
    float difference = xOverHigh - xOverLow;
    
    difference /=  rms;
    
    difference *= response_;
    
    integrator_ = integrator.process(difference);
    
    integrator_ = clamp2(integrator_, 0.0f, 1.0f); //Constrain integrator values
    
    integrator_ = powf_fast(integrator_, 2.0f);
}

float Spectral_Balance_Cheap::getTendencyHZ(){
    xOverFreq_ = integrator_ * nyquist_Adjusted_;
    return xOverFreq_;
}
