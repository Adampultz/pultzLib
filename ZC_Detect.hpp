//
//  ZC_Detect.hpp
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 23.03.21.
//

#ifndef ZC_Detect_h
#define ZC_Detect_h

#include <vector>
#include <cmath>
#include "CircularBuffer.h"
#include "Utilities.hpp"

namespace pultzLib {

template<class T>
class ZC_Detect {
public:
    ZC_Detect() {}                                                    // Default constructor
    ZC_Detect(int size);
              
    void initialise(float minFreq, float samplerate){
        minFreq_ = minFreq;
//        size_ = (1.0 / (minFreq_ / 2)) * samplerate;
        size_ = 2048;
        numSec_ = 0.5 / (size_ / samplerate);
        float sizeT = numSec_;
        buffer_.setup((int) size_, 0);
    };
    
    void process(T value){ // Write value to buffer at writepointer index.
        int oldVal = buffer_.getOldest();
        int biVal = zc.process(value);
        buffer_.write(biVal);
        numCrossings_ += biVal;
        numCrossings_ -= oldVal;
    }
    
    float getZC(){
        return numCrossings_ * numSec_;
    }
    
    ~ZC_Detect() {}                // Destructor

private:
    
    Zero_Crossing<float> zc;
    CircularBuffer<bool> buffer_;

    int counter;
    int sampleRate_;
    float size_;
    int numCrossings_;
    int zcCounter_;
    float numSec_;
    float minFreq_;
    
};

}


#endif /* ZC_Detect_h */
