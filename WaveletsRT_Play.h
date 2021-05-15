//
//  Wavelets_Play.hpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 01.01.21.
//  Copyright © 2021 JUCE. All rights reserved.
//
#pragma once

#ifndef WaveletsRT_Play_h
#define WaveletsRT_Play_h

#include <stdio.h>
#include <vector>
#include <cmath>

using namespace std;


class WaveletsRT_Play
{
public:
    WaveletsRT_Play() {}
    WaveletsRT_Play(int bufferSize, int wlNums, int increment, int repeats, bool useInterpolation);
    
    void setup(int bufferSize, int wlNums, int increment, int repeats, bool useInterpolation);
    
    void setAudioBuffer(std::vector<float>& audioBuffer, int index);
    
    void setWaveletInfo(std::vector<std::vector<float>>& infoBuffer, int index);
    
    void setRepeats(int repeats);
    
    void setWaveletNums(int wlNums);
    
    void setWaveletIncrement(float increment);
    
    void setValues(int wlNums, int increment, int repeats);
    
    int getNumWavelets();
    
    int getRepeats();
    
    int getIncrement();
    
    bool getChangeFlag();
    
    float getAudioBufLength();
    
    float process();
    
    ~WaveletsRT_Play() {}
    
private:
    vector<vector<float>>* audioBuffer_;
    vector<vector<vector<float>>>* waveletInfo_;
    int sampleRate_ = 48000;
    int bufferSize_ = sampleRate_ * 2.0;
    int waveletInfoSize_[3];
    int audioBufferSize_[3];
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
    int readIndex_;
    bool useInterpolation_;
    double interPolVal = 1.0;
    float fracCeil_ = 1.0;
    float fracFloorVal_ = 0.0;
    bool changeFlag_ = true;
};

#endif /* WaveletsRT_Play_h */
