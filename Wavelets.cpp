//
//  ZCAnalysis.cpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 28.12.20.
//  Copyright © 2020 JUCE. All rights reserved.
//

#include "Wavelets.h"

Wavelets::Wavelets(int minWindowSize, float ampThreshold){
    setup(minWindowSize = 10, ampThreshold = 0.2);
}

void Wavelets::setup(int minWindowSize, float ampThreshold){
    minWindowSize_ = minWindowSize;
    ampThreshold_ = ampThreshold;
    tempWaveletInfo_.push_back({0, 0, 0, 0, 0, 0, 0}); //Initialise array for Zero crossing data
} // ZC integer index 0
    // Fractional index 1
    //sample Value 2
    // Last sample value 3
    // Fractional length 4
    // Peak Amplitude 5
    // Mean amplitude 6

void Wavelets::waveletDetect(float sample){
    
    float x = sample;

    float absAmp = fabs(x);
    
    if(absAmp > localPeakAmp_)
        localPeakAmp_ = absAmp;
       
    gAmpAccum_ += absAmp; // Accumulate amplitude in Window
    
    sampleIndex_++; // We have already added the first index
        
    if (gPeriodCounter_ >= minWindowSize_ - 1){
        
        if (x_m1 < 0.0 && x >= 0.0){
        gAmpAccum_ /= gPeriodCounter_; // calculate mean amp of window (accumulated amplitude divided by length of past window
        tempWaveletInfo_[zcIndex_][5] = localPeakAmp_; // Peak amp of past window
        tempWaveletInfo_[zcIndex_][6] = gAmpAccum_; // Mean amp of past window
        tempWaveletInfo_.push_back({(float) sampleIndex_, 0, x, x_m1, 0, 0, 0}); // Add new ZC data to array: 0 = x (sample), 1 = sample index, 2 = direction
        meanAmp_ += gAmpAccum_; // Update mean amplitude with mean window amplitude
            
            if(localPeakAmp_ > peakAmp_)
            peakAmp_ = localPeakAmp_;

        gPeriodCounter_ = 0;
        localPeakAmp_ = 0;
        absAmp = 0;
        gAmpAccum_ = 0;
        zcIndex_++;
    }}
    
    x_m1 = x;
    gPeriodCounter_++;
}

void Wavelets::waveletShrink(){ // Think about Schmitt trigger // don't call shrink, something thresholding
    tempWaveletInfo_.erase(tempWaveletInfo_.begin());
    auto tempInfoSize = tempWaveletInfo_.size();
    meanAmp_ /= tempInfoSize;
    
    for(int n = 0; n < tempInfoSize - 1; n++){

        if(tempWaveletInfo_[n][5] < (meanAmp_ * ampThreshold_)){
            // If the peak amplitude of the window is less than the mean amplitude * 0.2 of the file, remove
            tempWaveletInfo_[n + 1][4] += tempWaveletInfo_[n + 1][4] - tempWaveletInfo_[n][4]; // Add fractional length of current window to that of next window
            if (tempWaveletInfo_[n][5] > tempWaveletInfo_[n + 1][5])
                tempWaveletInfo_[n + 1][5] = tempWaveletInfo_[n][5];
         //Replace next window's peak amplitude if amplitude of current window is greater
            tempWaveletInfo_[n + 1][6] = (tempWaveletInfo_[n + 1][6] + tempWaveletInfo_[n][6]) * 0.5; //Update next windows mean amplitude with the mean of n+1 and n
        } else {
            float lastIndex = tempWaveletInfo_[n][0] - 1;
            float lastVal = tempWaveletInfo_[n][3];
            float thisVal = tempWaveletInfo_[n][2];
            tempWaveletInfo_[n][1] = fractionalZC.process(lastIndex, lastVal, thisVal); // Add fractional crossing info
            waveletInfo_.push_back(tempWaveletInfo_[n]);
        }

        if (n > 0){
            tempWaveletInfo_[n][4] += tempWaveletInfo_[n][1] - tempWaveletInfo_[n - 1][1];}
        else{
            tempWaveletInfo_[n][4] += tempWaveletInfo_[n][1];
        }
    }
//    waveletInfo_ = tempWaveletInfo_; // Copy only qualified items to new vector

}

std::vector<std::vector<float>>& Wavelets::getWaveletInfo(){
    return waveletInfo_;    
}

float Wavelets::getMeanAmp(){
    meanAmp_ /= zcIndex_;
    return meanAmp_;
}

int Wavelets::getLength(){
    return sampleIndex_;
}
