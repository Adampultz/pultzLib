/*
 Amplitude Sync
 Designed for the FAAB. The algorithm attempts to synchronise its amplitude to the mean amplitude of its two neighbouring strings. See chapter 4 of my PhD thesis "A Continually Receding Horizon: Making, performing and improvising with semi-autonomous double bass feedback systems" (2023).
 */

#pragma once

#ifndef Amplitude_Sync_h
#define Amplitude_Sync_h

#include "Definitions.hpp"
#include "fundamentals.hpp"
#include "WaveTables.h"
#include "Chromosome.hpp"
#include "Operators.h"
#include "Utilities.hpp"


namespace pultzLib {

class Amplitude_Sync : public Definitions {
    
public:
    
    Amplitude_Sync() {};
    Amplitude_Sync(int size, int wtSize, float makeupGain);
    
    void init(int size, int wtSize, float makeupGain);
    
    void push(float val, int index);
    
    float getOscVal(int index);
    
    float getAmpDiffInd(int index);
    
    void setWeights();
    
    void compute();
    
    float getMeanAmp();
    
    float process(float sig, float amp, int index);
    
    ~Amplitude_Sync() {}
    
private:
    
    WaveTable_Sync<float>* oscillator;
    Variance<float> AmpVariance;
    AmpDiff ampDiff;
    Chromosome chromosome;

    int size_;
    int doubleSize_;
    float wtSize_;
    float makeUpGain_;
    float ampBalanceTarget_;
    float weightSum_;
    float ampBalance_;
    float weightSumNormalizeCoeff_;
   
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

