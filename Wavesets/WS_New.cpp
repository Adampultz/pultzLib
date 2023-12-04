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
    bufWrite_[0] = true;
    bufWrite_[1] = false;
        
    for(unsigned int i = 0; i < 2; i++){
        winsize_[i] = winsize;
        audioBuffer_[i].resize(maxwinsize);
        wavesetRTinfo_[i].resize(g_Nyquist, std::vector<float>(3));
        numRepeats_[i] = repeats;
        blockSize_[i] = wsNum;
        g_WsIncrement[i] = repeats;
        bufRead_[i] = false;
    }
    
}

void WS_New::wsWrite(float sig, float amp, float ampThreshold, float ampSensitivity){
    
    if (bufWrite_[0] == false && bufWrite_[1] == false){
        
    }
    
    if (bufWrite_[writeIdx_] == true){
        float source = sig;
        
        static int counterTest;
        
        // Returns true if waveset start detected, otherwise false
        bool wsTrue = wsDetect.process((float) sig, amp, ampThreshold, ampSensitivity);
        
        // If waveset start detected
        if (wsTrue == true){
            // register location (sampleCount_) in audio buffer and waveset index.
            wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][0] = sampleIndex_;
            wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][1] = sig;
            wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][2] = wsDetect.getPastVal();
            wsIndexCounter_[writeIdx_]++;  // Update waveset index
        }
        
//        sampleIndex_ = audioBuffer_[writeIdx_].getWritepointer();  // Get sample count from buffer object
        audioBuffer_[writeIdx_][sampleIndex_] = sig; // Write sample to buffer
        sampleIndex_++;
        counterTest++;
        // If the write pointer has exceeded the end of the buffer minus the minimum sample count for highest frequency (sample rate divided by maximum frequency)
        
        
        if (finish_ == true){
            
            // If write pointer has reached end of buffer
            if (sampleIndex_ >= winsize_[writeIdx_]){
                // Register index of last complete waveset
            
//                int lastWsIndx = clamp2((int) (wsIndexCounter_[writeIdx_] - 1), (int) 0, (int) g_Nyquist);
                int lastWsIndx = wsIndexCounter_[writeIdx_] - 1;
                counterTest;
                // Register start of last complete waveset
                lastWs_[writeIdx_] = wavesetRTinfo_[writeIdx_][lastWsIndx - 1][0];
                // Total num of complete wavesets equals index of last complete waveset
                numWs_[writeIdx_] = wsIndexCounter_[writeIdx_];
                
                bufWrite_[writeIdx_] = false; // Deactivate write for current buffer
                wsIndexCounter_[writeIdx_] = 0; // Reset waveset counter for current buffer
                wsDetect.reset();
                finish_ = false;
                
                firstSampleInBlock_[writeIdx_] = wavesetRTinfo_[writeIdx_][writeIdx_][0];
                unsigned int lastSampleIndex = blockSize_[writeIdx_];
                lastSampleInBlock_[writeIdx_] = wavesetRTinfo_[writeIdx_][lastSampleIndex][0];
                                
                sampleIndex_ = 0;
                // If this is not the first write cycle
                if (readActive_ == true){
                    readIdx_ = writeIdx_; // Pass write buffer index to read buffer index
                    
                    writeIdx_++;
//                    writeIdx_ &= wrapMask_;
                    if (writeIdx_ >= 2)
                        writeIdx_ = 0;
                    
                    writeActive_ = false;
                    // If the play object is still reading, the write buffer is now paused and will only resume once given permissin by the play object
                    
                } else { // If this is the first write cycle
                    readActive_ = true; // Activate read buffer (only after first write cycle)
                    readIdx_ = writeIdx_; // Pass write buffer index to read buffer index
                    
                    writeIdx_++;

                    
                    // Since this is the first write cycle, and nothing is currently being read, permission to read and write is immediately passed
                    bufWrite_[writeIdx_] = true;
                    bufRead_[readIdx_] = true;
                    
                }
            }
            // If finish_ = false;
        } else {
            // If write pointer has reached end of buffer
            if (sampleIndex_ >= winsize_[writeIdx_]){
                sampleIndex_ = 0;
                wsIndexCounter_[writeIdx_] = 0; // Reset waveset counter
            }
        }
    }
}

//void WS_New::wsWrite(float sig, float amp, float ampThreshold, float ampSensitivity){
//    float source = sig;
//
//    if (bufWrite_[writeIdx_] == true){
//
//        // Returns true if waveset start detected, otherwise false
//        bool wsTrue = wsDetect.process((float) sig, amp, ampThreshold, ampSensitivity);
//
//        // If waveset start detected
//        if (wsTrue == true){
//            // register location (sampleCount_) in audio buffer and waveset index.
//            wavesetRTinfo_[writeIdx_][wsIndexCounter_[writeIdx_]][0] = sampleIndex_;
//            wsIndexCounter_[writeIdx_]++;  // Update waveset index
//
//        }
//
//        audioBuffer_[writeIdx_].write(sig); // Write sample to buffer
//
//        // If the write pointer has exceeded the end of the buffer minus the minimum sample count for highest frequency (sample rate divided by maximum frequency)
//        sampleIndex_ = audioBuffer_[writeIdx_].getWritepointer();  // Get sample count from buffer object
//
//        if (finish_ == true){
//
//            // If write pointer has reached end of buffer
//            if (sampleIndex_ >= winsize_[writeIdx_] -1){
//                // Register index of last complete waveset
//                unsigned int lastWsIndx = wsIndexCounter_[writeIdx_] - 1;
//                // Register start of last complete waveset
//                lastWs_[writeIdx_] = wavesetRTinfo_[writeIdx_][lastWsIndx - 1][0];
//                // Total num of complete wavesets equals index of last complete waveset
//                numWs_[writeIdx_] = wsIndexCounter_[writeIdx_];
//
//                bufWrite_[writeIdx_] = false; // Deactivate write for current buffer
//                wsIndexCounter_[writeIdx_] = 0; // Reset waveset counter for current buffer
//                audioBuffer_[writeIdx_].resetWritePointer(); // Reset the write pointer for audio buffer
//                finish_ = false;
//
//                firstSampleInBlock_[writeIdx_] = wavesetRTinfo_[writeIdx_][0][0];
//                unsigned int lastSampleIndex = blockSize_[writeIdx_];
//                lastSampleInBlock_[writeIdx_] = wavesetRTinfo_[writeIdx_][lastSampleIndex][0] - 1;
//
//                // If this is not the first write cycle
//                if (readActive_ == true){
//                    readIdx_ = writeIdx_; // Pass write buffer index to read buffer index
//
//                    writeIdx_++;
//                    writeIdx_ &= wrapMask_;
//
//                    // If the play object is still reading, the write buffer is now paused and will only resume once given permissin by the play object
//
//                } else { // If this is the first write cycle
//                    readActive_ = true; // Activate read buffer (only after first write cycle)
//                    readIdx_ = writeIdx_; // Pass write buffer index to read buffer index
//
//                    writeIdx_++;
//                    writeIdx_ &= wrapMask_;
//
//                    // Since this is the first write cycle, and nothing is currently being read, permission to read and write is immediately passed
//                    bufWrite_[writeIdx_] = true;
//                    bufRead_[readIdx_] = true;
//
//                }
//            }
//            // If finish_ = false;
//        } else {
//            // If write pointer has reached end of buffer
//            if (sampleIndex_ >= winsize_[writeIdx_] -1)
//                wsIndexCounter_[writeIdx_] = 0; // Reset waveset counter
////                audioBuffer_[writeIdx_].resetWritePointer();
//
//        }
//    }
//}

bool WS_New::getWsFlag(){
    return wsTrue_[writeIdx_];
}

float WS_New::wsPlay(){

    float out = 0;

    if (readActive_ == true){
        if (bufRead_[readIdx_] == true){

            unsigned int bufferPos = sampleReadIdx_ + firstSampleInBlock_[readIdx_];
//            unsigned int lastSampBlock = lastSampleInBlock_[writeIdx_];
            unsigned int wsIdx = wsReadIndex_;

            out = audioBuffer_[readIdx_][bufferPos];

            sampleReadIdx_++;

            if (bufferPos >= lastSampleInBlock_[readIdx_]){
                
                sampleReadIdx_ = 0; // Reset sample index
                
                repeatIdx_++;

                if (repeatIdx_ >= numRepeats_[readIdx_]){
                    wsReadIndex_ += g_WsIncrement[readIdx_];
                    firstSampleInBlock_[readIdx_] = wavesetRTinfo_[readIdx_][wsReadIndex_][0];
                    unsigned int lastSampleIndex = wsReadIndex_ + blockSize_[readIdx_];
                    lastSampleInBlock_[readIdx_] = wavesetRTinfo_[readIdx_][lastSampleIndex][0] - 1;
//                        wsReadIndex_ = 0;
                    repeatIdx_ = 0;
                    
                    if (wsReadIndex_ >= numWs_[readIdx_]){
                        wsReadIndex_ = 0;
                        firstSampleInBlock_[readIdx_] = wavesetRTinfo_[readIdx_][wsReadIndex_][0];
                        unsigned int lastSampleIndex = wsReadIndex_ + blockSize_[readIdx_];
                        lastSampleInBlock_[readIdx_] = wavesetRTinfo_[readIdx_][lastSampleIndex][0] - 1;
                        
                    }
                }
            }
        }
    }

    return out;
}

//float WS_New::wsPlay(){
//
//    float out = 0;
//
//    if (readActive_ == true){
//
//        if (bufRead_[readIdx_] == true){
//
//            // Iterate over number of wavesets in buffer
//            for (unsigned int w = 0; w < numWs_[readIdx_]; w++){
//                if (w >= numWs_[readIdx_] - (blockSize_[readIdx_] + 1)){
//                    finish_ = true;}
//
//                // Iterate over number of repeats of each waveset block
//                for (unsigned int n = 0; n < numRepeats_[readIdx_]; n++){
//                    //                iterate over number of wavesets in each block
//                    unsigned int lastSampleIndex = w + blockSize_[readIdx_];
//                    unsigned int firstSampleInBlock = wavesetRTinfo_[readIdx_][w][0];
//                    unsigned int lastSampleInBlock = wavesetRTinfo_[readIdx_][lastSampleIndex][0];
//                    //                    Total number of samples in each block
////                    int wsSize = lastSampleInBlock - firstSampleInBlock;
//                    //                    Iterate over total number of samples in each block
//
//                    for (unsigned int s = firstSampleInBlock; s < lastSampleInBlock; s++){
//                        out = audioBuffer_[readIdx_].read(s);
//
//                        if (finish_ == true){
//                            // If this is the last repetition
//                            if (n >= numRepeats_[readIdx_] - 1){
//                                // If the sample counter has reached the last sample in the last block, trigger buffer change
//                                if (s >= lastSampleInBlock -1){
//                                    finish_ = false;
//                                    bufRead_[readIdx_] = false; // Deactivate current read buffer
//                                    bufWrite_[readIdx_] = true; // Set write object to write into current buffer
//                                    writeIdx_ = readIdx_;
//                                    // Increment and wrap read index
//                                    readIdx_++;
//                                    readIdx_ &= wrapMask_;
//
//                                    bufRead_[readIdx_] = true; // Activate read object to read from next buffer
//
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//        }
//    }
//    return out;
//}

void WS_New::wsMonitor(){
    
}


