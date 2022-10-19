// Class for analysing wavesets in real-time
// By Adam Pultz Melbye, 2021

#include "Wavesets_RT.h"

using namespace pultzLib;

Wavesets_RT::Wavesets_RT(int analysisWindow, int minWindowSize, float ampThreshold){
    setup(analysisWindow = 48000, minWindowSize = 10, ampThreshold = 0.2);
}

void Wavesets_RT::setup(int analysisWindow, int minWindowSize, float ampThreshold){
    analysisWindow_ = analysisWindow;
    minWindowSize_ = minWindowSize;
    maxVectorSize_ = analysisWindow_;
    ampThreshold_ = ampThreshold;
    wavesetInfo_.resize(maxVectorSize_, std::vector<float>(6));//Initialise array for Zero crossing data
} // ZC integer index 0
    // Fractional index 1
    //sample Value 2
    // Last sample value 3
    // Fractional length 4
    // Peak Amplitude 5
    // Mean amplitude 6

void Wavesets_RT::clearInfoBuffer(){
    gPeriodCounter_ = 0;
    localPeakAmp_ = 0;
    gAmpAccum_ = 0;
    zcIndex_ = 0;
}

int Wavesets_RT::getWsPos(int index){
    return wavesetInfo_[index][0];
}



int Wavesets_RT::getLastZC(){
    return wavesetInfo_[numWavesets_][0];
}

float Wavesets_RT::getWsGateVal(int index){
    return wavesetInfo_[index][7];
}

void Wavesets_RT::detect(float sample, int index){
    
    index_ = index;
    float x = sample;

    float absAmp = fabsf_neon(x);
    
    if(absAmp > localPeakAmp_)
        localPeakAmp_ = absAmp;
       
    gAmpAccum_ += absAmp; // Accumulate amplitude in Window
    gPeriodCounter_++; // Is this the wrong place for period counter?

        
        if (x_m1 < 0.0 && x >= 0.0){
            if (localPeakAmp_ > (lastMeanAmp_ * ampThreshold_)){
                gAmpAccum_ /= gPeriodCounter_; // calculate mean amp of window (accumulated amplitude divided by length of past window

            if (zcIndex_ > 0){
            wavesetInfo_[zcIndex_ - 1][5] = localPeakAmp_; // Peak amp of past window
            wavesetInfo_[zcIndex_ - 1][6] = gAmpAccum_;
            }// Mean amp of past window
                
            wavesetInfo_[zcIndex_] = {(float) index_, 0.0f, x, x_m1, 0.0f, 0.0f, 0.0f}; // Add new ZC data to array: 0 = x (sample), 1 = sample index, 2 = direction
            meanAmp_ += gAmpAccum_; // Update mean amplitude with mean window amplitude
                
                if(localPeakAmp_ > peakAmp_)
                peakAmp_ = localPeakAmp_;

            lastMeanAmp_ = gAmpAccum_;
            gPeriodCounter_ = 0;
            localPeakAmp_ = 0;
            gAmpAccum_ = 0;
            zcIndex_++;
            }
    }
    
    x_m1 = x;
}

void Wavesets_RT::filter(){ // Think about Schmitt trigger
 
    numWavesets_ = zcIndex_;
    zcIndex_ = 0;
    lastMeanAmp_ = 0;
    gPeriodCounter_ = 0;
    localPeakAmp_ = 0;
    gAmpAccum_ = 0;
}

std::vector<std::vector<float>>& Wavesets_RT::getWavesetInfo(){
    return wavesetInfo_;    
}

float Wavesets_RT::getMeanAmp(){
    meanAmp_ /= zcIndex_;
    return meanAmp_;
}

int Wavesets_RT::getLength(){
    return numWavesets_;
}
