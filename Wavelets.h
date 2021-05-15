//
//  ZCAnalysis.hpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 28.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//
// Zero Crossing analysis based on Alberto De Campo's Wavelet

#pragma once

#ifndef Wavelets_h
#define Wavelets_h

#include <stdio.h>
#include <vector>
#include "frac_ZC.h"

class Wavelets
{
public:
    Wavelets(){}
    Wavelets(int minWindowSize, float ampThreshold);
    
    void setup(int minWindowSize, float ampThreshold);
    
    void waveletDetect(float sample);
    
    void waveletShrink();
    
    std::vector<std::vector<float>>& getWaveletInfo();
    
    float getMeanAmp();
    
    int getLength();
    
    ~Wavelets() {}
    
private:
    
    frac_ZC fractionalZC;
    std::vector<std::vector<float>> tempWaveletInfo_;
    std::vector<std::vector<float>> waveletInfo_;
    int minWindowSize_ = 10;
    float ampThreshold_ = 0;
    float x_m1 = 0; // Last Sample
    float meanAmp_ = 0;
    float peakAmp_ = 0;
    float localPeakAmp_ = 0;
    float gAmpAccum_ = 0;
    int gPeriodCounter_;
    int zcIndex_ = 0;
    int audioFileSize_;
    int sampleIndex_ = 0;
};

#endif /* Wavelets_h */
