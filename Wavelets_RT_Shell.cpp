//
//  Wavelets_RT_Shell.cpp
//  A class for encapsulating the analysis and playback of wavelets
//
//  Created by Adam Pultz Melbye on 16.01.21.

#include "Wavelets_RT_Shell.h"

Wavelets_RT_Shell::Wavelets_RT_Shell(int sampleRate, float analysisWindow, int minWindowSize, float ampThreshold,
                                     int repeats, int increase, int wlNum){
    setup(sampleRate, analysisWindow, minWindowSize, ampThreshold, repeats, increase, wlNum);
}

void Wavelets_RT_Shell::setup(int sampleRate, float analysisWindow, int minWindowSize, float ampThreshold, int repeats,
                              int increase, int wlNum){
    waveletsRT = new WaveletsRT[3];
    sampleRate_ = sampleRate;
    analysisWindow_ = analysisWindow * sampleRate_;
    minWinSize_ = minWindowSize;
    ampThreshold_ = ampThreshold;
    numRepeats_ = repeats;
    waveletIncrement_ = increase;
    wlNum_ = wlNum;
    audioBuffer_.resize(3, vector<float>(analysisWindow_));
    waveletRTinfo_.resize(3, vector<vector<float>>(1));
    for (int i = 0; i < 3; i++){
        waveletsRT[i].setup(analysisWindow_, minWinSize_, ampThreshold_);
        waveletRTinfo_[i].push_back({0, 0, 0, 0, 0, 0});
    }
    bufChange_ = true;
    wlPlay_ = false;
    mutePlay = true;
}

void Wavelets_RT_Shell::setVariables(int repeats, int increase, int wlNum){
    numRepeats_ = repeats;
    waveletIncrement_ = increase;
    wlNum_ = wlNum;
}

int Wavelets_RT_Shell::getInfoLength(){
    return waveletInfoSize_[audioBufWriteIndex_];
}

float Wavelets_RT_Shell::process(float source){
    
    float source_ = source;
    
    audioBuffer_[audioBufWriteIndex_][audioBufWriteCounter_] = source_;
    waveletsRT[audioBufWriteIndex_].detect(source_, audioBufWriteCounter_);
    
    audioBufWriteCounter_++;
    if (audioBufWriteCounter_ > analysisWindow_ - 1){
        audioBufWriteCounter_ = 0;
        
        if (bufChange_ == true){ // If read counter has moved to next buffer
            waveletsRT[audioBufWriteIndex_].filter();
//            waveletRTinfo_[audioBufWriteIndex_] = waveletsRT[audioBufWriteIndex_].getWaveletInfo();
            
            waveletInfoSize_[audioBufWriteIndex_] = waveletsRT[audioBufWriteIndex_].getLength();
//            lastZC_[audioBufWriteIndex_] = (int) waveletRTinfo_[audioBufWriteIndex_][waveletInfoSize_[audioBufWriteIndex_]][0];
//            lastZC_[audioBufWriteIndex_] = (int) waveletsRT[audioBufWriteIndex_].getWlPos(waveletInfoSize_[audioBufWriteIndex_]);
            lastZC_[audioBufWriteIndex_] = (int) waveletsRT[audioBufWriteIndex_].getLastZC();
//            waveletsRT[audioBufWriteIndex_].clearInfoBuffer();
            audioBufWriteIndex_++;
            
            if (audioBufWriteIndex_ > 2)
                audioBufWriteIndex_ = 0;
            audioBufReadIndex_ = audioBufWriteIndex_ + 1;
            if (audioBufReadIndex_ > 2)
                audioBufReadIndex_ = 0;
        
            
            if (mutePlay == true){
            if (audioBufWriteIndex_ > 1){
                wlPlay_ = true; // Set to true when write buffer 0 and 1 are full
                bufChange_ = false;
                mutePlay = false;
                readPointer_ = (int) waveletsRT[audioBufReadIndex_].getWlPos(1);
            }
            }
            
            if (wlPlay_ == true)
            bufChange_ = false;
            
        } else {
            if (wlPlay_ == true)
            waveletsRT[audioBufWriteIndex_].clearInfoBuffer();
        }
    }
    
    if (wlPlay_ == true) {
        float harm = audioBuffer_[audioBufReadIndex_][initTransPos_ + transPosPointer_];
        float wlPlayback_ =  (audioBuffer_[audioBufReadIndex_][readPointer_]);
//        float wlPlayback_ = harm;
        readPointer_++;
        transPosPointer_ += 3;
//        harmPoint_ = transPosPointer_ * 1;
        
        if (transPosPointer_ >= nextWavelet_){
            transPosPointer_ = 0;
        }
   
        if(readPointer_ >=  (int) waveletsRT[audioBufReadIndex_].getWlPos(nextWl_ID_)){
            if (reapeatIndex_ >= numRepeats_ - 1){
                wl_Accum_ += waveletIncrement_;
                nextWl_ID_ = wlNum_ + wl_Accum_;
                reapeatIndex_ = 0;
                harmPoint_ = 0;
                transPosPointer_ = 0;
                float nextVal = waveletsRT[audioBufReadIndex_].getVal(wl_Accum_);
     //           fractionalZero = fractionalZC.process(readPointer_ - 1, wlPlayback_, nextVal);
                fractionalZero = nextVal -  wlPlayback_;
                actualFractionalZero = waveletsRT[audioBufReadIndex_].getFractionalZC(wl_Accum_);
                fractionalDifference_ = fabs(fractionalZero - actualFractionalZero) * 0.5;
            } else {
                reapeatIndex_++;
                harmPoint_ = 0;
                transPosPointer_ = 0;
                fractionalDifference_ = 0;
            }
            
       if (readPointer_ > lastZC_[audioBufReadIndex_] - 1){ // If readPointer is equal to or larger than the last negative to positive zc
           
           bufChange_ = true;
           
           audioBufReadIndex_++;
           if (audioBufReadIndex_ > 2)
               audioBufReadIndex_ = 0;
           
           wl_Accum_ = 1;
           readPointer_ = (int) waveletsRT[audioBufReadIndex_].getWlPos(1);
           initTransPos_ = readPointer_;
           reapeatIndex_ = 1;
           nextWl_ID_ = wlNum_ + 1;
                              
       } else {
           readPointer_ = (int) waveletsRT[audioBufReadIndex_].getWlPos(wl_Accum_);
           nextWavelet_ = (int) waveletsRT[audioBufReadIndex_].getWlPos(wl_Accum_ + 1);
           initTransPos_ = readPointer_;
           targetGateVal_ = waveletsRT[audioBufReadIndex_].getWlGateVal(wl_Accum_);
           
           if(gateVal_ < targetGateVal_){
               gateVal_ *= 1.2;
           } else if (gateVal_ > targetGateVal_){
               gateVal_ *= 0.9;
           } else if (gateVal_ == targetGateVal_) {
               gateVal_ = targetGateVal_;
           }
           
           if (gateVal_ < 0.2)
               gateVal_ = 0.2;
           
       }
    }
        return wlPlayback_ - fractionalDifference_;
    } else{
        return 0;
    }
    
}

