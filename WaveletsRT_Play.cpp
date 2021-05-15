//
//  Wavelets_Play.cpp
//  LoopingAudioSampleBufferAdvancedTutorial - App
//
//  Created by Adam Pultz Melbye on 01.01.21.
//  Copyright © 2021 JUCE. All rights reserved.
//

#include "WaveletsRT_Play.h"

WaveletsRT_Play::WaveletsRT_Play(int bufferSize, int wlNums, int increment, int repeats, bool useInterpolation){
    setup(bufferSize, wlNums, increment, repeats, useInterpolation);
}

void WaveletsRT_Play::setup(int bufferSize, int wlNums, int increment, int repeats, bool useInterpolation = true){
    bufferSize_ = bufferSize;
    audioBuffer_->resize(3, vector<float>(bufferSize_));
    waveletInfo_->resize(3, vector<vector<float>>(1));
    waveletNum_ = wlNums;
    waveletIncrement_ = increment;
    numRepeats_ = repeats;
    useInterpolation_ = useInterpolation;
    nextWl_ID_ = waveletNum_;
    changeFlag_ = true;
//    for (int i = 0; i < 3; i++){
//        (*waveletInfo_)[i].push_back({0, 0, 0, 0, 0, 0});
//    }
}

void WaveletsRT_Play::setAudioBuffer(std::vector<float>& audioBuffer, int index){
    int index_ = index;
//    (*audioBuffer_)[0] = &audioBuffer;
//    audioBufferSize_[index] = audioBuffer_->size();
}

void WaveletsRT_Play::setWaveletInfo(std::vector<std::vector<float>>& infoBuffer, int index){
//    (*waveletInfo_)[0].push_back(&infoBuffer);
    waveletInfoSize_[index] = waveletInfo_->size();
    readPointer_ = 0; // To do: change to start from first ZC
}

void WaveletsRT_Play::setWaveletNums(int wlNums){
    waveletNum_ = wlNums;
}

void WaveletsRT_Play::setWaveletIncrement(float increment){
    waveletIncrement_ = waveletNum_ - (waveletNum_ * increment);
    if (waveletIncrement_ < 1)
        waveletIncrement_ = 1;
}

void WaveletsRT_Play::setRepeats(int repeats){
    numRepeats_ = repeats;
}

void WaveletsRT_Play::setValues(int wlNums, int increment, int repeats){
    waveletNum_ = wlNums;
    waveletIncrement_ = increment;
    numRepeats_ = repeats;
}

int WaveletsRT_Play::getNumWavelets(){
    return wlNumPrint;
}

int WaveletsRT_Play::getIncrement(){
    return wlIncPrint;
}

int WaveletsRT_Play::getRepeats(){
    return wlRepPrint;
}

float WaveletsRT_Play::getAudioBufLength(){
    return audioBuffer_->size();
}

bool WaveletsRT_Play::getChangeFlag(){
    return changeFlag_;
}

float WaveletsRT_Play::process (){
    
    float source = (*audioBuffer_)[readIndex_][readPointer_] * fracCeil_ + fracFloorVal_;
   
    readPointer_++;
   
//   if(readPointer_ >= (int) (*waveletInfo_)[readIndex_][nextWl_ID_][0]){
//       if (reapeatIndex_ >= numRepeats_ - 1){
//           wl_Accum_ += waveletIncrement_;
//           if (wl_Accum_ >= waveletInfoSize_[readIndex_])
//               wl_Accum_ = waveletIncrement_;
//           nextWl_ID_ = waveletNum_ + wl_Accum_ + 1;
//           if (nextWl_ID_ >= waveletInfoSize_[readIndex_])
//               nextWl_ID_ -= waveletInfoSize_[readIndex_];
//           reapeatIndex_ = 0;
//           wlRepPrint = numRepeats_;
//           wlNumPrint = waveletNum_;
//           wlIncPrint = waveletIncrement_;
//       } else {
//           reapeatIndex_++;
//       }
//       if (readPointer_ >= (int) (*waveletInfo_)[readIndex_][waveletInfoSize_ - 1][0]){ // If readPointer is equal to or larger than the last negative to positive zc
//           readPointer_ = 0;
//           reapeatIndex_ = 0;
//           fracCeil_ = 1.0; // We cannot interpolate since we do not know the new value. Yet
//           fracFloorVal_ = 0;
//           changeFlag_ = true;
//       } else {
//       readPointer_ = (int) (*waveletInfo_)[readIndex_][wl_Accum_][0];
//       float nextVal = (*waveletInfo_)[readIndex_][wl_Accum_][2];
//       float fracFloor = fabs((nextVal - source) * 1.0);
//       fracCeil_ = 1.0 - fracFloor;
//       fracFloorVal_ = source * fracFloor;}
//   }
   
   return source;
    
    
}
