//
//  Wavesets_RT_Shell.hpp

//  Created by Adam Pultz Melbye on 16.01.21.


#ifndef Wavesets_RT_Shell_h
#define Wavesets_RT_Shell_h

#include <stdio.h>
#include "Wavesets_RT.h"
#include <cmath>

namespace pultzLib {

class Wavesets_RT_Shell
{
public:
    Wavesets_RT_Shell(){}
    Wavesets_RT_Shell(int sampleRate, float analysisWindow, int minWindowSize, float ampThreshold, int repeats, int increase, int wsNum);
    
    void setup(int sampleRate, float analysisWindow, int minWindowSize, float ampThreshold, int repeats, int increase, int wsNum);
    
    void setVariables(int repeats, int increase, int wsNum);
    
    void setAnalysisWindow(float analysisWindow);
    
    void setRepeats(int repeats);
   
    void setIncrement(int increment);
   
    void setWsNum(int wsNum);
    
    int getInfoLength();
    
    float process(float source);
    
    ~Wavesets_RT_Shell() {}
    
private:
    Wavesets_RT* wavesets_RT;
    
    std::vector<std::vector<std::vector<float>>> wavesetRTinfo_;
    std::vector<std::vector<float>> audioBuffer_;
    unsigned int nextWs_ID_ = 10;
    unsigned int numRepeats_ = 1;
    unsigned int wavesetIncrement_ = 1;
    unsigned int reapeatIndex_;
    unsigned int ws_Accum_ = 1;
    unsigned int wavesetInfoSize_[3];
    unsigned int wsNum_ = 10;
    unsigned int minWinSize_;

    float ampThreshold_;
    unsigned int lastZC_[3];
    bool bufChange_;
    bool wsPlay_;
    bool mutePlay;
    int sampleRate_ = 48000;
    int analysisWindow_[3];
    float analysisWindowSec_;
    int readPointer_;
    int audioBufWriteIndex_ = 0;
    int audioBufReadIndex_ = 0;
    int audioBufWriteCounter_ = 0; // Start at index, immediately before read index
};
}

#endif /* Wavesets_RT_Shell_h */
