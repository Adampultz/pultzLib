//
//  ZCAnalysis.hpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 28.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//
// Zero Crossing analysis based on Alberto De Campo's Wavelet

#pragma once

#ifndef WaveletsRT_h
#define WaveletsRT_h

#include <stdio.h>
#include <vector>
#include "frac_ZC.h"

using namespace std;

class WaveletsRT
{
public:
    WaveletsRT(){}
    WaveletsRT(int analysisWindow, int minWindowSize, float ampThreshold);
    
    void setup(int analysisWindow, int minWindowSize, float ampThreshold);
    
    void detect(float sample, int index);
    
    void clearInfoBuffer();
    
    void filter();
    
    vector<vector<float>>& getWaveletInfo();
    
    int getWlPos(int index);
    
    int getLastZC();
    
    float getVal(int index);
    
    float getFractionalZC(int index);
    
    float getWlGateVal(int index);
    
    float getMeanAmp();
    
    int getLength();
    
    ~WaveletsRT() {}
    
private:
    vector<vector<float>> tempWaveletInfo_;
    vector<vector<float>> waveletInfo_;
    frac_ZC fractionalZC;
    int analysisWindow_;
    int index_ = 0;
    int minWindowSize_ = 10;
    int maxVectorSize_;
    int wlCount_;
    int numWavelets_;
    float ampThreshold_ = 0;
    float x_m1 = 0; // Last Sample
    float meanAmp_ = 0;
    float peakAmp_ = 0;
    float localPeakAmp_ = 0;
    float gAmpAccum_ = 0;
    float lastMeanAmp_;
    int gPeriodCounter_;
    int zcIndex_ = 0;
    int audioFileSize_;
    int sampleIndex_ = 0;
    int meanAmpThreshCount = 0;
};

#endif /* WaveletsRT_h */
