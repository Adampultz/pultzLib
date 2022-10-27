/*
 Class for calculating number of zero-crossings for use as a crude frequency detector.
 */

#ifndef ZC_Detect_h
#define ZC_Detect_h

#include <vector>
#include <cmath>
#include "CircularBuffers.h"
#include "Utilities.hpp"

namespace pultzLib {

template<class T>
class ZC_Detect {
public:
    ZC_Detect() {}
    ZC_Detect(int size);
              
    void initialise(float minFreq, float samplerate){
        minFreq_ = minFreq;
        size_ = 2048;
        numSec_ = 0.5f / (size_ / samplerate);
        buffer_.setup((int) size_);
    };
    
    void process(T value){ // Write value to buffer at writepointer index.
        bool oldVal = buffer_.getOldest();
        bool biVal = zc.process(value);
        buffer_.write(biVal);
        numCrossings_ += biVal;
        numCrossings_ -= oldVal;
    }
    
    float getZC(){
        return numCrossings_ * numSec_;
    }
    
    ~ZC_Detect() {}              

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
