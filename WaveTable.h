//
//  WaveTable.h
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 10.05.21.
//

#ifndef WaveTable_h
#define WaveTable_h

#include <vector>
#include <cmath>

namespace pultzLib {

template<class T>
class WaveTable {
public:
    WaveTable() {}                                                    // Default constructor
    WaveTable(int size);
              
    void setup(float frequency, int size, int samplerate){
        frequency_ = frequency;
        sampleRate_ = samplerate;
        inverseSampleRate_ = 1.0 / sampleRate_;
        size_ = size;
        wrapMask_ = bufferLength_ - 1;
        writePointer_ = 0;
        increment_ = frequency_ * size_ / sampleRate_;
        phase_ = 0;
        buffer_.resize(size);
    };
    
    void write(T value){ // Write value to buffer at writepointer index.
        buffer_[writePointer_] = value;
        writePointer_++;
    };
    
    void setFrequency(float frequency){ // Write value to buffer at writepointer index
        increment_ = frequency * size_ * inverseSampleRate_;
    };
    
    T read(){
        T out;
        
        int indexBelow = floorf(phase_);
        int indexAbove = indexBelow + 1;
        indexAbove &= wrapMask_;
        float fractionAbove = phase_ - indexBelow;
        float fractionBelow = 1 - fractionAbove;
        phase_ += increment_;
        float testPhase =  buffer_[indexBelow];
        if (phase_ >= size_)
            phase_ -= size_;
        
//        out = (fractionBelow * buffer_[indexBelow]) + (fractionAbove * buffer_[indexAbove]); // Linear
        out = buffer_[phase_]; // Linear

        
        return out;
    }
    
    ~WaveTable() {}                // Destructor

private:
    
    std::vector<T> buffer_;
    int size_;
    float frequency_;
    int sampleRate_;
    float increment_;
    float inverseSampleRate_;
    int writePointer_;
    int readPointer_;
    int bufferLength_; // Adjusting the actual buffer to a power of two for masking
    int wrapMask_;
    float phase_;
    
};

}


#endif /* WaveTable_h */
