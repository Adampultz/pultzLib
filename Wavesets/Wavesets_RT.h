
#pragma once

#ifndef Wavesets_RT_h
#define Wavesets_RT_h

#include <stdio.h>
#include <vector>
#include "Fast_Math.hpp"

namespace pultzLib {

class Wavesets_RT
{
public:
    Wavesets_RT(){}
    Wavesets_RT(int analysisWindow, int minWindowSize, float ampThreshold);
    
    void init(int analysisWindow, int minWindowSize, float ampThreshold);
    
    void detect(float sample, int index);
    
    void clearInfoBuffer();
    
    void filter();
    
    std::vector<std::vector<float>>& getWavesetInfo();
    
    int getWsPos(int index);
    
     int getLastZC();
    
    float getWsGateVal(int index);
    
    float getMeanAmp();
    
    int getLength();
    
    ~Wavesets_RT() {}
    
private:
    std::vector<std::vector<float>> tempWavesetInfo_;
    std::vector<std::vector<float>> wavesetInfo_;
    unsigned int analysisWindow_;
    unsigned int index_ = 0;
    unsigned int minWindowSize_ = 10;
    unsigned int maxVectorSize_;
    unsigned int numWavesets_;
    float ampThreshold_ = 0;
    float x_m1 = 0;
    float meanAmp_ = 0;
    float peakAmp_ = 0;
    float localPeakAmp_ = 0;
    float gAmpAccum_ = 0;
    float lastMeanAmp_;
    unsigned int gPeriodCounter_;
    unsigned int zcIndex_ = 0;
};

}

#endif /* Wavesets_RT_h */
