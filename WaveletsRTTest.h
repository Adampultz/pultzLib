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

using namespace std;

class WaveletsRT
{
public:
    WaveletsRT(){}
    WaveletsRT(int minWindowSize, float ampThreshold);
    
    void setup(int minWindowSize, float ampThreshold);
    
    void detect(float sample, int index);
    
    void filter();
    
    vector<vector<float>>& getWaveletInfo();
    
    float getMeanAmp();
    
    int getLength();
    
    ~WaveletsRT() {}
    
private:
    vector<vector<float>> tempWaveletInfoVect_;
    vector<vector<float>> waveletInfoVect_;
    vector<vector<float>> waveletSubVect_;
    int index_ = 0;
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

#endif /* WaveletsRT_h */
