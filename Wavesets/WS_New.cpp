//
//  WS_New.cpp
//  Basic_DSP - App
//
//  Created by Adam Pultz Melbye on 23/11/2023.
//

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
        wavesetRTinfo_[i].resize(infoBufSize, std::vector<float>(4));
        numRepeats_[i] = repeats;
        blockSize_[i] = wsNum;
        g_WsIncrement[i] = repeats;
        bufRead_[i] = false;
        bufWrite_[i] = false;
    }
    
    bufWrite_[0] = true;
    
}

void WS_New::wsWrite(float sig, float amp, float ampThreshold, float ampSensitivity){

    if (bufWrite_[writeIdx_] == true){
        float source = sig;
                
        // Returns true if waveset start detected, otherwise false
        bool wsTrue = wsDetect.process((float) sig, amp, ampThreshold, ampSensitivity);
        
        // If waveset start detected
        if (wsTrue == true){
            // register location (sampleCount_) in audio buffer and waveset index.
            wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][0] = sampleIndex_;
            wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][1] = sig;
            wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][2] = wsDetect.getPastVal();
            
            
            // If this is not the first write cycle or the first waveset detection, save the value of
            //the second sample in the previous waveset, to use for linear interpolation
            if (bufFull_ == true){
                // Last waveset index, wrapped bitwise
                unsigned int lastWsIdx = wsIndexCounter_[writeIdx_] - 1;
                lastWsIdx &= infoSizeWrapMask_;
                // Location of second sample in last waveset
                unsigned int idx = wavesetRTinfo_[writeIdx_][lastWsIdx][0] + 1;
                wavesetRTinfo_[writeIdx_][lastWsIdx][3] = audioBuffer_[writeIdx_][idx];
            } else if (wsIndexCounter_[writeIdx_] != 0){
                unsigned int idx = wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_] - 1][0] + 1;
                wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_] - 1][3] = audioBuffer_[writeIdx_][idx];
            }
            
            numWs_[writeIdx_]++; // Increment total number of wavesets
            
//            Increment index counter for waveset and wrap bitwise
            wsIndexCounter_[writeIdx_]++;
            wsIndexCounter_[writeIdx_] &= infoSizeWrapMask_;
        }
        
        if (bufFull_ == true){
            // If the readpointer reaches the sample index of the oldest waveset in the buffer
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

//void WS_New::wsWrite(float sig, float amp, float ampThreshold, float ampSensitivity){
//
//    if (bufWrite_[writeIdx_] == true){
//        float source = sig;
//
//        static int counterTest;
//
//        // Returns true if waveset start detected, otherwise false
//        bool wsTrue = wsDetect.process((float) sig, amp, ampThreshold, ampSensitivity);
//
//        // If waveset start detected
//        if (wsTrue == true){
//            // register location (sampleCount_) in audio buffer and waveset index.
//            wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][0] = sampleIndex_;
//            wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][1] = sig;
//            wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][2] = wsDetect.getPastVal();
//            wsIndexCounter_[writeIdx_]++;  // Update waveset index
//        }
//
////        sampleIndex_ = audioBuffer_[writeIdx_].getWritepointer();  // Get sample count from buffer object
//        audioBuffer_[writeIdx_][sampleIndex_] = sig; // Write sample to buffer
//        sampleIndex_++;
//        counterTest++;
//        // If the write pointer has exceeded the end of the buffer minus the minimum sample count for highest frequency (sample rate divided by maximum frequency)
//
//
//        if (finish_ == true){
//
//            // If write pointer has reached end of buffer
//            if (sampleIndex_ >= winsize_[writeIdx_]){
//                // Register index of last complete waveset
//
////                int lastWsIndx = clamp2((int) (wsIndexCounter_[writeIdx_] - 1), (int) 0, (int) g_Nyquist);
//                int lastWsIndx = wsIndexCounter_[writeIdx_] - 1;
//                counterTest;
//                // Register start of last complete waveset
//                lastWs_[writeIdx_] = wavesetRTinfo_[writeIdx_][lastWsIndx - 1][0];
//                // Total num of complete wavesets equals index of last complete waveset
//                numWs_[writeIdx_] = wsIndexCounter_[writeIdx_];
//
//                bufWrite_[writeIdx_] = false; // Deactivate write for current buffer
//                wsIndexCounter_[writeIdx_] = 0; // Reset waveset counter for current buffer
//                wsDetect.reset();
////                finish_ = false;
//
//                firstSampleInBlock_[writeIdx_] = wavesetRTinfo_[writeIdx_][writeIdx_][0];
//                unsigned int lastSampleIndex = blockSize_[writeIdx_];
//                lastSampleInBlock_[writeIdx_] = wavesetRTinfo_[writeIdx_][lastSampleIndex][0];
//
//                sampleIndex_ = 0;
//                // If this is not the first write cycle
//                if (readActive_ == true){
//                    readIdx_ = writeIdx_; // Pass write buffer index to read buffer index
//
//                    writeIdx_++;
////                    writeIdx_ &= wrapMask_;
//                    if (writeIdx_ >= 3)
//                        writeIdx_ = 0;
//
//                    writeActive_ = false;
//                    // If the play object is still reading, the write buffer is now paused and will only resume once given permissin by the play object
//
//                } else { // If this is the first write cycle
//                    bufWrite_[writeIdx_] = false;
//
//                    if (writeIdx_++ < 2){
////                        bufWrite_[writeIdx_] = true;
//
//                    } else {
//                        readActive_ = true; // Activate read buffer (only after first write cycle)
//                        finish_ = false;
//                    }
//
//                    bufWrite_[writeIdx_] = true;
//
//                }
//            }
//            // If finish_ = false;
//        } else {
//            // If write pointer has reached end of buffer
//            if (sampleIndex_ >= winsize_[writeIdx_]){
//                sampleIndex_ = 0;
//                wsIndexCounter_[writeIdx_] = 0; // Reset waveset counter
//            }
//        }
//    }
//}


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


void WS_New::wsMonitor(){
    
}


