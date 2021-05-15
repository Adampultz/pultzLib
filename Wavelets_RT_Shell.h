//
//  Wavelets_RT_Shell.hpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 16.01.21.
//  Copyright © 2021 JUCE. All rights reserved.
//

#ifndef Wavelets_RT_Shell_h
#define Wavelets_RT_Shell_h

#include <stdio.h>
#include "WaveletsRT.h"
#include "frac_ZC.h"

using namespace std;

class Wavelets_RT_Shell
{
public:
    Wavelets_RT_Shell(){}
    Wavelets_RT_Shell(int sampleRate, float analysisWindow, int minWindowSize, float ampThreshold, int repeats, int increase, int wlNum);
    
    void setup(int sampleRate, float analysisWindow, int minWindowSize, float ampThreshold, int repeats, int increase, int wlNum);
    
    void setVariables(int repeats, int increase, int wlNum);
    
    int getInfoLength();
    
    float process(float source);
    
    ~Wavelets_RT_Shell() {}
    
private:
    WaveletsRT* waveletsRT;
    frac_ZC fractionalZC;
    
    vector<vector<vector<float>>> waveletRTinfo_;
    vector<vector<float>> audioBuffer_;
    int nextWl_ID_ = 10;
    int numRepeats_ = 1;
    int waveletIncrement_ = 1;
    int reapeatIndex_;
    int wl_Accum_ = 1;
    int waveletInfoSize_[3];
    int wlNum_ = 10;
    int minWinSize_;
    int tripCount_;

    float peakAmp_;
    float meanAmp_;
    float fracCeil_;
    float fracFloorVal_;
    float ampThreshold_;
    int lastZC_[3];
    bool bufChange_;
    bool wlPlay_;
    bool mutePlay;
    int sampleRate_ = 48000;
    int analysisCounter_ = 0;
    int analysisWindow_;
    int readPointer_;
    int anWriteIndex_ = 0;
    int audioBufWriteIndex_ = 0;
    int audioBufReadIndex_ = 0;
    int audioBufWriteCounter_ = 0; // Start at index, immediately before read index
    int audioBufReadPointer_ = 0;
    int initTransPos_ = 0;
    int transPosPointer_ = 0;
    int harmPoint_ = 0;
    double gateVal_ = 1;
    float targetGateVal_ = 1;
    int nextWavelet_ = 1;
    float fractionalDifference_;
    float fractionalZero;
    float actualFractionalZero;
    
};

#endif /* Wavelets_RT_Shell_h */
