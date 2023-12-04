//  Wavesets_RT_Shell.cpp
//  A class for encapsulating the analysis and playback of wavelets
//
//  Created by Adam Pultz Melbye on 16.01.21.

#include "Wavesets_RT_Shell.h"

using namespace pultzLib;

Wavesets_RT_Shell::Wavesets_RT_Shell(float analysisWindow, int minWindowSize, float ampThreshold, int repeats, int increase, int wsNum){
    init(analysisWindow, minWindowSize, ampThreshold, repeats, increase, wsNum);
}

void Wavesets_RT_Shell::init(float analysisWindow, int minWindowSize, float ampThreshold, int repeats,
                              int increase, int wsNum){
    analysisWindowSec_ = analysisWindow;
    wavesets_RT = new Wavesets_RT[3];
    minWinSize_ = minWindowSize;
    ampThreshold_ = ampThreshold;
    numRepeats_ = repeats;
    wavesetIncrement_ = increase;
    wsNum_ = wsNum;
    wavesetRTinfo_.resize(3, std::vector<std::vector<float>>(1));
    
    for (int i = 0; i < 3; i++){
        analysisWindow_[i] = analysisWindowSec_ * g_SampleRate;
        wavesets_RT[i].init(analysisWindow_[i], minWinSize_, ampThreshold_);
        wavesetRTinfo_[i].push_back({0, 0, 0, 0, 0, 0});
    }
    
    audioBuffer_.resize(3, std::vector<float>(analysisWindow_[0]));
    bufChange_ = true;
    wsPlay_ = false;
    mutePlay = true;
}

void Wavesets_RT_Shell::setVariables(int repeats, int increase, int wlNum){
    numRepeats_ = repeats;
    wavesetIncrement_ = increase;
    wsNum_ = wlNum;
}

void Wavesets_RT_Shell::setAnalysisWindow(float analysisWindow){
	if (bufChange_ == false){
    analysisWindowSec_ = analysisWindow;
	// analysisWindow_[audioBufWriteIndex_] = analysisWindowSec_ *  sampleRate_; // Compute length of next analysis window in samples
	}
}

void Wavesets_RT_Shell::setRepeats(int repeats){
	numRepeats_ = repeats;
}

void Wavesets_RT_Shell::setIncrement(int increment){
	wavesetIncrement_ = increment;
}

void Wavesets_RT_Shell::setWsNum(int wsNum){
	wsNum_ = wsNum;
}

int Wavesets_RT_Shell::getInfoLength(){
    return wavesetInfoSize_[audioBufWriteIndex_];
}

float Wavesets_RT_Shell::process(float source){
    
    float source_ = source;
    
    audioBuffer_[audioBufWriteIndex_][audioBufWriteCounter_] = source_; // Fill buffer at index with current sample value
    wavesets_RT[audioBufWriteIndex_].detect(source_, audioBufWriteCounter_); 
    
    audioBufWriteCounter_++; // Update write index

    if (audioBufWriteCounter_ >= analysisWindow_[audioBufWriteIndex_]){ // If write index is larger than or equal to analysis window
        audioBufWriteCounter_ = 0; // Reset write counter
        
        if (bufChange_ == true){ // If read counter has moved to next buffer
            wavesets_RT[audioBufWriteIndex_].filter();
            wavesetInfoSize_[audioBufWriteIndex_] = wavesets_RT[audioBufWriteIndex_].getLength() - 1; // Get length of current 
			lastZC_[audioBufWriteIndex_] = (int) wavesets_RT[audioBufWriteIndex_].getWsPos(wavesetInfoSize_[audioBufWriteIndex_]); // Get position of last zero-crossing
            
            // Figure out how to compute the last zero crossing within the changing buffer sizes
            
            audioBufWriteIndex_++; // Update write buffer index
            
            if (audioBufWriteIndex_ > 2)
                audioBufWriteIndex_ = 0;
            audioBufReadIndex_ = audioBufWriteIndex_ + 1;
            if (audioBufReadIndex_ > 2)
                audioBufReadIndex_ = 0;
                
            analysisWindow_[audioBufWriteIndex_] = analysisWindowSec_ *  g_SampleRate; // Compute length of next analysis window in samples
            
            if (mutePlay == true){ // If flag is set for initial buffer fill
                if (audioBufWriteIndex_ > 1){
                    wsPlay_ = true; // Set to true when write buffer 0 and 1 are full
                    bufChange_ = false;
                    mutePlay = false;
                }
            }
            
            if (wsPlay_ == true)
                bufChange_ = false;
            
        } else {
            if (wsPlay_ == true)
            wavesets_RT[audioBufWriteIndex_].clearInfoBuffer();
        }
    }
    
    if (wsPlay_ == true) { // If flag is set for read operations
        float wsPlayback_ = audioBuffer_[audioBufReadIndex_][readPointer_]; // Read sample value stored at index in buffer
        readPointer_++; // Increment read pointer
   
        if(readPointer_ >=  (int) wavesets_RT[audioBufReadIndex_].getWsPos(nextWs_ID_)){ // If read pointer is larger than or equal to next waveset index

            if (reapeatIndex_ >= numRepeats_ - 1){ // If this is the last repeat
                ws_Accum_ += wavesetIncrement_; // Increment waveset 
                nextWs_ID_ = wsNum_ + ws_Accum_; // Calculate next waveset index
                reapeatIndex_ = 0; // Reset repeat index
            } else { // If this is NOT the last repeat
                reapeatIndex_++; // Increment repeat index
            }
            
       if (readPointer_ >= lastZC_[audioBufReadIndex_]){ // If readPointer is equal to or larger than the last negative to positive zc
           bufChange_ = true;
           
           audioBufReadIndex_++;
           
           if (audioBufReadIndex_ > 2)
               audioBufReadIndex_ = 0;
           
        	 ws_Accum_ = 1;
			readPointer_ = (int) wavesets_RT[audioBufReadIndex_].getWsPos(1);

           reapeatIndex_ = 1;
           nextWs_ID_ = wsNum_ + 1;

                              
       } else {
		    readPointer_ = (int) wavesets_RT[audioBufReadIndex_].getWsPos(ws_Accum_);
       }
    }
        return wsPlayback_;
    } else{
        return 0;
    }
    
}

