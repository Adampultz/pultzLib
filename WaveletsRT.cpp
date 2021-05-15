// Class for analysing wavelets in real-time
// By Adam Pultz Melbye, 2021

#include "WaveletsRT.h"

WaveletsRT::WaveletsRT(int analysisWindow, int minWindowSize, float ampThreshold){
    setup(analysisWindow = 48000, minWindowSize = 10, ampThreshold = 0.2);
}

void WaveletsRT::setup(int analysisWindow, int minWindowSize, float ampThreshold){
    analysisWindow_ = analysisWindow;
    minWindowSize_ = minWindowSize;
    maxVectorSize_ = analysisWindow_;
    ampThreshold_ = ampThreshold;
//    tempWaveletInfo_.resize(maxVectorSize_, vector<float>(6));
    waveletInfo_.resize(maxVectorSize_, vector<float>(7));//Initialise array for Zero crossing data
} // ZC integer index 0
    // Fractional index 1
    //sample Value 2
    // Last sample value 3
    // Fractional length 4
    // Peak Amplitude 5
    // Mean amplitude 6
    // Gate value 7

int WaveletsRT::getWlPos(int index){
    return waveletInfo_[index][0];
}

int WaveletsRT::getLastZC(){
    return waveletInfo_[numWavelets_][0];
}

float WaveletsRT::getWlGateVal(int index){
    return waveletInfo_[index][7];
}

float WaveletsRT::getVal(int index){
    return waveletInfo_[index][2];
}

float WaveletsRT::getFractionalZC(int index){
    return waveletInfo_[index][1];
}

void WaveletsRT::clearInfoBuffer(){
    gPeriodCounter_ = 0;
    localPeakAmp_ = 0;
    gAmpAccum_ = 0;
    zcIndex_ = 0;
}

void WaveletsRT::detect(float sample, int index){
    
    index_ = index;
    float x = sample;

    float absAmp = fabs(x);
    
    if(absAmp > localPeakAmp_)
        localPeakAmp_ = absAmp;
       
    gAmpAccum_ += absAmp; // Accumulate amplitude in Window
    gPeriodCounter_++; // Is this the wrong place for period counter?


        
        if (x_m1 < 0.0 && x >= 0.0){
            if (localPeakAmp_ > (lastMeanAmp_ * ampThreshold_)){
                gAmpAccum_ /= gPeriodCounter_; // calculate mean amp of window (accumulated amplitude divided by length of past window

            if (zcIndex_ > 0){
            waveletInfo_[zcIndex_ - 1][5] = localPeakAmp_; // Peak amp of past window
            waveletInfo_[zcIndex_ - 1][6] = gAmpAccum_;
                if (gAmpAccum_ < 0.001){
                    if (meanAmpThreshCount > 2){
                        waveletInfo_[zcIndex_ - 1][7] = 0.2;
                    } else {
                        meanAmpThreshCount++;
                        waveletInfo_[zcIndex_ - 1][7] = 1;
                    }
                } else {
                    waveletInfo_[zcIndex_ - 1][7] = 1;
                    meanAmpThreshCount = 0;
                }
            }// Mean amp of past window
                
//                float fracZC = fractionalZC.process((int) index_ - 1, x, x_m1);
                float fracZC = x - x_m1;

            waveletInfo_[zcIndex_] = {(float) index_, fracZC, x, x_m1, 0, 0, 0, 0}; // Add new ZC data to array: 0 = x (sample), 1 = sample index, 2 = direction
            meanAmp_ += gAmpAccum_; // Update mean amplitude with mean window amplitude
                
                if(localPeakAmp_ > peakAmp_)
                peakAmp_ = localPeakAmp_;

            lastMeanAmp_ = gAmpAccum_;
            gPeriodCounter_ = 0;
            localPeakAmp_ = 0;
            gAmpAccum_ = 0;
            zcIndex_++;
            }
    }
    
    x_m1 = x;
}

void WaveletsRT::filter(){ // Think about Schmitt trigger // don't call shrink, something thresholding
 
    numWavelets_ = zcIndex_;
    zcIndex_ = 0;
    lastMeanAmp_ = 0;
    gPeriodCounter_ = 0;
    localPeakAmp_ = 0;
    gAmpAccum_ = 0;
}

vector<vector<float>>& WaveletsRT::getWaveletInfo(){
    return waveletInfo_;    
}

float WaveletsRT::getMeanAmp(){
    meanAmp_ /= zcIndex_;
    return meanAmp_;
}

int WaveletsRT::getLength(){
    return numWavelets_;
}
