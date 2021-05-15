//
//  Wavelets_Play.cpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 01.01.21.
//  Copyright © 2021 JUCE. All rights reserved.
//

#include "Wavelets_Play.h"

Wavelets_Play::Wavelets_Play(int wlNums, int increment, int repeats, bool useInterpolation){
//    setup(wlNums, increment, repeats,useInterpolation);
}

void Wavelets_Play::setup(int wlNums, int increment, int repeats, bool useInterpolation = true){
    waveletNum_ = wlNums;
    waveletIncrement_ = increment;
    numRepeats_ = repeats;
    useInterpolation_ = useInterpolation;
    nextWl_ID_ = waveletNum_;
    transCoeff_ = 1.0;
}

void Wavelets_Play::setAudioBuffer(std::vector<float>& audioBuffer){
    audioBuffer_ = &audioBuffer;
    audioBufferSize_ = audioBuffer_->size();
}

void Wavelets_Play::setWaveletInfo(std::vector<std::vector<float>>& infoBuffer){
    waveletInfo_ = &infoBuffer;
    waveletInfoSize_ = waveletInfo_->size();
}

void Wavelets_Play::setWaveletNums(int wlNums){
    waveletNum_ = wlNums;
}

void Wavelets_Play::setWaveletIncrement(float increment){
    waveletIncrement_ = waveletNum_ - (waveletNum_ * increment);
    if (waveletIncrement_ < 1)
        waveletIncrement_ = 1;
}

void Wavelets_Play::setRepeats(int repeats){
    numRepeats_ = repeats;
}

void Wavelets_Play::setValues(int wlNums, int increment, int repeats){
    waveletNum_ = wlNums;
    waveletIncrement_ = increment;
    numRepeats_ = repeats;
}

int Wavelets_Play::getNumWavelets(){
    return wlNumPrint;
}

int Wavelets_Play::getIncrement(){
    return wlIncPrint;
}

int Wavelets_Play::getRepeats(){
    return wlRepPrint;
}

float Wavelets_Play::getAudioBufLength(){
    return audioBuffer_->size();
}

float Wavelets_Play::process (){
   
    float source = (*audioBuffer_)[readPointer_];
//    int lastVal_ = readPointer_;
//    float lastSource = (*audioBuffer_)[lastVal_ - 1];
    
    readPointer_++;
//    if(readPointer_ ==  ((int) (*waveletInfo_)[nextWl_ID_][0]) - 1){
////       float transValNeg = (*audioBuffer_)[readPointer_];
//        wl_Accum_ += waveletIncrement_;
//        if (wl_Accum_ >= waveletInfoSize_)
//            wl_Accum_ = waveletIncrement_;
//        float transValpos = (*waveletInfo_)[wl_Accum_][2];
//        int id = (*waveletInfo_)[wl_Accum_][0] - 1;
//        float fracCrossA =  fractional_ZC.process(id, source, transValpos);
//        float fracCrossB = (*waveletInfo_)[wl_Accum_][1];
//        transCoeff_ = 1.0 + fabs(fracCrossB - fracCrossA);
//        source *= transCoeff_;
//    } else {
//        transCoeff_ = 1.0;
//    }
//

       
   if(readPointer_ >=  ((int) (*waveletInfo_)[nextWl_ID_][0])){
       if (reapeatIndex_ >= numRepeats_ - 1){

            wl_Accum_ += waveletIncrement_;
            if (wl_Accum_ >= waveletInfoSize_)
                wl_Accum_ = waveletIncrement_;


           nextWl_ID_ = waveletNum_ + wl_Accum_;
           if (nextWl_ID_ >= waveletInfoSize_)
               nextWl_ID_ -= waveletInfoSize_;
           reapeatIndex_ = 0;
       } else {
           reapeatIndex_++;
       }
       readPointer_ = (int) (*waveletInfo_)[wl_Accum_][0];

   } else {
//       fracCeil_ = 1.0;
//       fracFloorVal_ = 0;
   }
   
   
   return source;
}
