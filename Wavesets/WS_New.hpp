//
//  WS_New.hpp
//  Basic_DSP - App
//
//  Created by Adam Pultz Melbye on 23/11/2023.
//

#ifndef WS_New_hpp
#define WS_New_hpp

#include <stdio.h>
#include <vector>
#include <cmath>
#include "Definitions.hpp"
#include "Utilities.hpp"
#include "fundamentals.hpp"

using namespace pultzLib;

class WS_New {
public:
    WS_New(){}
    
    void init(int winsize, int maxwinsize, float ampThreshold, int repeats, int increase, int wsNum);
    
    void audioBufPush(float sig);
    
    float process(float sig, float amp, float ampThreshold, float ampSensitivity);
    
    void wsWrite(float sig, float amp, float ampThreshold, float ampSensitivity);
    
    void wsMonitor();
    
    void setPrimaryVals(float sig);
    
    void setSecondaryVals(int lastWsIdx);
    
    void setRepeats(float val, float inMin, float inMax, float outMin, float outMax);
    
    void getRepeats(float val, float inMin, float inMax, float outMin, float outMax);
    
    float wsPlay();
    
    bool getWsFlag();
    
    float process(float sig);
    
private:
    
    WavesetDetect<float> wsDetect;
    
    std::vector<std::vector<std::vector<float>>> wavesetRTinfo_;
    std::vector<float> audioBuffer_[2];
    
    unsigned int winsize_[2];
    unsigned int numWs_[2];
    unsigned int blockSize_[2] {1, 1};
//    unsigned int bufindex_;
    unsigned int wsInfoIndex_;
    unsigned int wsIndexCounter_[2];
    unsigned int sampleCount_[2];
    unsigned int lastWs_[2];
    unsigned int sampleIndex_;
    unsigned int readIdx_ = 0;
    unsigned int writeIdx_ = 0;
    unsigned int wrapMask_ = 1;
    unsigned int numRepeats_[2] {1, 1};
    unsigned int wsReadIndex_;
    unsigned int repeatIdx_;
    unsigned int g_WsIncrement[2] {1, 1};
    unsigned int oldestWsIdx_[2];
    unsigned int longestWs_;
    
    unsigned int firstSampleInBlock_[2];
    unsigned int lastSampleIndex_[2];
    unsigned int lastSampleInBlock_[2];
    unsigned int newestSampleInBuffer_[2];
    
    unsigned int sampleReadIdx_ = 0;
    unsigned int infoSizeWrapMask_ = 0;
    unsigned int audioBufferReadIdx_;
    unsigned int maxWsLength_;
    
    float out_;
    float y_m1;
    
    bool wsTrue_[2];
    bool readActive_;
    bool finish_;
    bool bufRead_[2] {false, false};
    bool bufWrite_[2] {true, false};
    bool writeActive_ = true;
    bool bufFull_ = false;
    bool interp_ = false;
};

#endif /* WS_New_hpp */
