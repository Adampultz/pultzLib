//
//  Wavelets_Play.hpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 01.01.21.
//  Copyright © 2021 JUCE. All rights reserved.
//
#pragma once

#ifndef Wavelets_Play_h
#define Wavelets_Play_h

#include <stdio.h>
#include <vector>
#include <cmath>
#include "frac_ZC.h"


class Wavelets_Play
{
public:
    Wavelets_Play() {}
    Wavelets_Play(int wlNums, int increment, int repeats, bool useInterpolation);
    
    void setup(int wlNums, int increment, int repeats, bool useInterpolation);
    
    void setAudioBuffer(std::vector<float>& audioBuffer);
    
    void setWaveletInfo(std::vector<std::vector<float>>& InfoBuffer);
    
    void setRepeats(int repeats);
    
    void setWaveletNums(int wlNums);
    
    void setWaveletIncrement(float increment);
    
    void setValues(int wlNums, int increment, int repeats);
    
    int getNumWavelets();
    
    int getRepeats();
    
    int getIncrement();
    
    float getAudioBufLength();
    
    float getCoeff();
    
    float process();
    
    ~Wavelets_Play() {}
    
private:
    frac_ZC fractional_ZC;
    std::vector<float>* audioBuffer_;
    std::vector<std::vector<float>>* waveletInfo_;
    int waveletInfoSize_;
    int audioBufferSize_;
    int readPointer_ = 0;
    int waveletNum_ = 10; // Number of wavelets per iteration
    int waveletIncrement_ = 1; // Number of wavelets to advance per new increment
    int numRepeats_ = 1; // Number of repeats per cycle
    int reapeatIndex_ = 0; // Keeping track of repeats
    int nextWl_ID_ = 10;
    int wl_Accum_ = 0;
    int wlNumPrint;
    int wlIncPrint;
    int wlRepPrint;
    float lastVal_ = 0;
    float transCoeff_;
    bool useInterpolation_;
    double interPolVal = 1.0;
    double fracCeil_ = 1.0;
    double fracFloorVal_ = 0.0;
};

#endif /* Wavelets_Play_h */
