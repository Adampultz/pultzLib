//
//  Amplitude_Sync.hpp
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 28.06.22.
//

#pragma once

#ifndef Amplitude_Sync_h
#define Amplitude_Sync_h

#include "fundamentals.hpp"
#include "WaveTables.h"

namespace pultzLib {

class Amplitude_Sync{
    
public:
    
    Amplitude_Sync() {};
    Amplitude_Sync(int size, int wtSize, float sampleRate, float makeupGain);
    
    void init(int size, int wtSize, float sampleRate, float makeupGain);
    
    void push(float val, int index);
    
    float getOscVal(int index);
    
    float getAmpDiffInd(int index);
    
    void setWeights(vector<float>& weights);
    
    void compute();
    
    float process(float sig, int index);
    
    ~Amplitude_Sync() {}
    
private:
    
    WaveTable_Sync<float>* oscillator;

    int size_;
    int doubleSize_;
    float wtSize_;
    float makeUpGain_;
    float ampBalanceTarget_;
    float weightSum_;
    float ampBalance_;
   
    std::vector<int> leftIndex_;
    std::vector<int> rightIndex_;
    std::vector<int> weightsLeftIndex_;
    std::vector<int> weightsRightIndex_;
    std::vector<float> amplitudes_;
    std::vector<float> tempAmpDiff_;
    std::vector<float> oscValues_;
    std::vector<float> weights_;
    std::vector<float> ampDifferences_;
};

}

#endif /* Amplitude_Sync_h */

