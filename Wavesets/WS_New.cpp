//
//  WS_New.cpp
//  Basic_DSP - App
//
//  Created by Adam Pultz Melbye on 23/11/2023.
//

// TODO: Calculate waveset lentgh variance per write buffer
// TODO: Calculate maximum ws length

#include "WS_New.hpp"

void WS_New::init(int winsize, int maxwinsize, float ampThreshold, int repeats, int increase, int wsNum){
    wavesetRTinfo_.resize(2);
    readActive_ = false;
    finish_ = true;
    
    unsigned int infoBufSize = nextPow2(maxwinsize / 10);
    infoSizeWrapMask_ = infoBufSize - 1;
        
    for(unsigned int i = 0; i < 2; i++){
        winsize_[i] = winsize;
        audioBuffer_[i].resize(maxwinsize);
        wavesetRTinfo_[i].resize(infoBufSize, std::vector<float>(6));
        numRepeats_[i] = repeats;
        blockSize_[i] = wsNum;
        g_WsIncrement[i] = repeats;
        bufRead_[i] = false;
        bufWrite_[i] = false;
    }
    
    bufWrite_[0] = true;
    
}

void WS_New::setPrimaryVals(float sig){
    wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][0] = sampleIndex_;
    wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][1] = sig;
    wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][2] = wsDetect.getPastVal();
}

void WS_New::setSecondaryVals(int lastWsIdx){
    unsigned int idx = wavesetRTinfo_[writeIdx_][lastWsIdx][0] + 1;
    wavesetRTinfo_[writeIdx_][lastWsIdx][3] = audioBuffer_[writeIdx_][idx];
    wavesetRTinfo_[writeIdx_][lastWsIdx][4] = wsDetect.getLength();
    wavesetRTinfo_[writeIdx_][lastWsIdx][5] = wsDetect.getMeanAmp();
}

void WS_New::wsWrite(float sig, float amp, float ampThreshold, float ampSensitivity){

    if (bufWrite_[writeIdx_] == true){
        float source = sig;
                
        // Returns true if waveset start detected, otherwise false
        bool wsTrue = wsDetect.process((float) sig, amp, ampThreshold, ampSensitivity);
        
        // If waveset start detected
        if (wsTrue == true){
            this->setPrimaryVals(sig);
                        
            // If this is not the first write cycle or the first waveset detection, save the value of
            //the second sample in the previous waveset, to use be used for linear interpolation
            
            unsigned int lastWsIdx = wsIndexCounter_[writeIdx_] - 1;
            
            if (bufFull_ == true){
                // Last waveset index, wrapped bitwise
                lastWsIdx &= infoSizeWrapMask_;
                // Location of second sample in last waveset
                this->setSecondaryVals(lastWsIdx);
            } else if (wsIndexCounter_[writeIdx_] != 0){
                this->setSecondaryVals(lastWsIdx);
            } else if (wsIndexCounter_[writeIdx_] == 0){
                this->setPrimaryVals(sig);
            }
            
            numWs_[writeIdx_]++; // Increment total number of wavesets
            
//            Increment index counter for waveset and wrap bitwise
            wsIndexCounter_[writeIdx_]++;
            wsIndexCounter_[writeIdx_] &= infoSizeWrapMask_;
        }
        
        if (bufFull_ == true){
            // If the write pointer reaches the sample index of the oldest waveset in the buffer
            if (sampleIndex_ == wavesetRTinfo_[writeIdx_][oldestWsIdx_[writeIdx_]][0]){
                oldestWsIdx_[writeIdx_]++; // Increment oldest waveset
                oldestWsIdx_[writeIdx_] &= infoSizeWrapMask_;
                
                numWs_[writeIdx_]--; // Decrement total number of wavesets
            }
        }
        
//        sampleIndex_ = audioBuffer_[writeIdx_].getWritepointer();  // Get sample count from buffer object
        audioBuffer_[writeIdx_][sampleIndex_] = sig; // Write sample to buffer
        sampleIndex_++;
        // If the write pointer has exceeded the end of the buffer minus the minimum sample count for highest frequency (sample rate divided by maximum frequency)
        
        if (sampleIndex_ >= winsize_[writeIdx_]){
            
            if (readActive_ == false){
                
                firstSampleInBlock_[readIdx_] = wavesetRTinfo_[writeIdx_][oldestWsIdx_[writeIdx_]][0];
                unsigned int lastWsIndex = oldestWsIdx_[writeIdx_] + blockSize_[writeIdx_];
                lastWsIndex &= infoSizeWrapMask_;
                lastSampleInBlock_[readIdx_] = wavesetRTinfo_[writeIdx_][lastWsIndex][0] - 1;
                unsigned int newestWsinBuffer = wsIndexCounter_[writeIdx_] - 1; // Last full waveset detected
                newestWsinBuffer &= infoSizeWrapMask_;
                
                newestSampleInBuffer_[writeIdx_] = wavesetRTinfo_[writeIdx_][newestWsinBuffer][0] - 1;
                if (newestSampleInBuffer_[writeIdx_] < 0)
                    newestSampleInBuffer_[writeIdx_] = winsize_[writeIdx_] - 1;
        
                audioBufferReadIdx_ = firstSampleInBlock_[readIdx_];
                wsIndexCounter_[writeIdx_] = 0;
                oldestWsIdx_[writeIdx_] = 0;
                bufWrite_[writeIdx_] = false;
                bufRead_[readIdx_] = true;
                readActive_ = true;
                writeIdx_ = 1;
                bufWrite_[writeIdx_] = true;
                bufFull_ = false;
                wsDetect.reset();
                
            } else if (bufFull_ == false){
                bufFull_ = true;
            }
            
            sampleIndex_ = 0;
        }
    }
}

void WS_New::setRepeats(float val, float inMin, float inMax, float outMin, float outMax){
    int numReps = (int) mapLin((float) val, inMin, inMax, outMin, outMax, true);
}


bool WS_New::getWsFlag(){
    return wsTrue_[writeIdx_];
}

float WS_New::wsPlay(){

    float out = 0;

//    if (readActive_ == true){
        if (bufRead_[readIdx_] == true){

//            unsigned int bufferPos = sampleReadIdx_ + firstSampleInBlock_[readIdx_];
//            unsigned int lastSampBlock = lastSampleInBlock_[writeIdx_];
            unsigned int wsIdx = wsReadIndex_;

            out = audioBuffer_[readIdx_][audioBufferReadIdx_];
            
            if (interp_ == true){
                out = linInterpNeg2Pos((double) wavesetRTinfo_[readIdx_][wsReadIndex_][2], (double) wavesetRTinfo_[readIdx_][wsReadIndex_][3], (double) 0.5);
                interp_ = false;
            }
            
            sampleReadIdx_++;
            
            audioBufferReadIdx_ = sampleReadIdx_ + firstSampleInBlock_[readIdx_];
            
            if (audioBufferReadIdx_ >= winsize_[readIdx_]){
                audioBufferReadIdx_ = 0;
                sampleReadIdx_ = 0;
                firstSampleInBlock_[readIdx_] = 0;
            }
            
            if (audioBufferReadIdx_ == newestSampleInBuffer_[readIdx_]){
                
                interp_ = true;
                bufWrite_[writeIdx_] = false;
                bufRead_[readIdx_] = false;
                bufFull_ = false;

                sampleReadIdx_ = 0;
                repeatIdx_ = 0;
                wsReadIndex_ = 0;
                numWs_[readIdx_] = 0;

                writeIdx_ = readIdx_;

                readIdx_++;
                readIdx_ &= 1;

                firstSampleInBlock_[readIdx_] = wavesetRTinfo_[readIdx_][oldestWsIdx_[readIdx_]][0];
                unsigned int lastWsIndex = oldestWsIdx_[readIdx_] + blockSize_[readIdx_];
                lastWsIndex &= infoSizeWrapMask_;
                lastSampleInBlock_[readIdx_] = wavesetRTinfo_[readIdx_][lastWsIndex][0] - 1;
                
                unsigned int newestWsinBuffer = wsIndexCounter_[readIdx_] - 1; // Last full waveset detected
                newestWsinBuffer &= infoSizeWrapMask_;
                
                newestSampleInBuffer_[readIdx_] = wavesetRTinfo_[readIdx_][newestWsinBuffer][0] - 1;
                
                if (newestSampleInBuffer_[readIdx_] < 0)
                    newestSampleInBuffer_[readIdx_] = winsize_[readIdx_] - 1;
                
                wsDetect.reset();
                audioBufferReadIdx_ = firstSampleInBlock_[readIdx_];
                wsIndexCounter_[writeIdx_] = 0;
                oldestWsIdx_[writeIdx_] = 0;
                numWs_[writeIdx_] = 0;
                sampleIndex_ = 0;
                bufWrite_[writeIdx_] = true;
                bufRead_[readIdx_] = true;
                
            } else if (audioBufferReadIdx_ == lastSampleInBlock_[readIdx_]){
                    
                interp_ = true;
                sampleReadIdx_ = 0; // Reset sample index
                    
                repeatIdx_++;
                    
                if (repeatIdx_ >= numRepeats_[readIdx_]){
                    wsReadIndex_ += g_WsIncrement[readIdx_];
                    wsReadIndex_ &= infoSizeWrapMask_;
                        
                    firstSampleInBlock_[readIdx_] = wavesetRTinfo_[readIdx_][wsReadIndex_][0];
                    unsigned int lastSampleIndex = wsReadIndex_ + blockSize_[readIdx_];
                    lastSampleInBlock_[readIdx_] = wavesetRTinfo_[readIdx_][lastSampleIndex][0] - 1;
                        
                    if (lastSampleInBlock_[readIdx_] < 0)
                        lastSampleInBlock_[readIdx_] = winsize_[readIdx_] - 1;
                                                        
                    repeatIdx_ = 0;
                    audioBufferReadIdx_ = firstSampleInBlock_[readIdx_];
                        
                } else {
                    audioBufferReadIdx_ = sampleReadIdx_ + firstSampleInBlock_[readIdx_];
                }
            }
        }
    return out;
}




