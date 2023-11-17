/*
 A selection of wavetables with linear interpolation
 */

#ifndef WaveTables_h
#define WaveTables_h

#include <vector>
#include <cmath>
#include "fundamentals.hpp"

namespace pultzLib {

/*
 Wavetable for use with the Amplitude Sync algorithm
 */

template<class T>
class WaveTable_Sync {
public:
    WaveTable_Sync() {}
              
    void setup(float frequency, int size, int samplerate){
        std::vector<float> tempBuffer_;
        tempBuffer_.resize(size * 2);
        frequency_ = frequency;
        sampleRate_ = samplerate;
        inverseSampleRate_ = 1.0 / sampleRate_;
        size_ = size;
        halfSize_ = size / 2;
        quadrant_ = size / 4;
        thirdQuadrant_ = size - quadrant_;
        bufferLength_ = (unsigned int) (pow(2, ceil(log(size_) / log(2)))); // Round buffer length to nearest power of 2
        wrapMask_ = bufferLength_ - 1;
        writePointer_ = 0;
        increment_ = frequency_ * size_ / sampleRate_;
        phase_ = 0;
        buffer_.resize(size_ + 1);
        
        for(unsigned int n = 0; n < size_; n++ ){
        buffer_[n] = sin(2 * M_PI * (float)n / (float)size_);
        };
    };
    
    void write(T value){ // Write value to buffer at writepointer index.
        buffer_[writePointer_] = value;
        writePointer_++;
        
    };
    
    void setFrequency(T frequency){ // Write value to buffer at writepointer index
        frequency_ = frequency;
        increment_ = frequency_ * size_ * inverseSampleRate_;
    };
    
    T read(){
        T out;
        
        int indexBelow = floorf(phase_);
        int indexAbove = indexBelow + 1;
        indexAbove &= wrapMask_;
        
        T fractionAbove = phase_ - indexBelow;
        T fractionBelow = 1.0 - fractionAbove;
        
        if(phase_ < quadrant_){
            phase_ += increment_;
            while(phase_ <= 0){
                phase_ += size_;}
                }
            else if(phase_ > thirdQuadrant_){
                phase_ += increment_;
            while(phase_ >= size_){
                phase_ -= size_;
                }
            }
        else if(phase_ < halfSize_){
            phase_ -= increment_;}
            else if(phase_ > halfSize_){
                phase_ -= increment_;}
        
        out = (fractionBelow * buffer_[indexBelow]) + (fractionAbove * buffer_[indexAbove]);
        
        return out;
    }
    
    ~WaveTable_Sync() {}

private:
    
    std::vector<T> buffer_;
    
    int size_;
    int quadrant_;
    int thirdQuadrant_;
    float frequency_;
    int sampleRate_;
    float increment_;
    float inverseSampleRate_;
    int writePointer_;
    int readPointer_;
    int bufferLength_;
    int wrapMask_;
    float phase_;
    int oscDirection;
    bool halfSize_;
    
};

template<class T>
class WaveTable {
public:
    WaveTable() {}
              
    void init(float frequency, int size, int samplerate){
        std::vector<float> tempBuffer_;
        tempBuffer_.resize(size * 2);
        frequency_ = frequency;
        sampleRate_ = samplerate;
        inverseSampleRate_ = 1.0 / sampleRate_;
        size_ = size;
        bufferLength_ = (unsigned int) (pow(2, ceil(log(size_) / log(2)))); // Round buffer length to nearest power of 2
        wrapMask_ = bufferLength_ - 1;
        writePointer_ = 0;
        increment_ = frequency_ * size_ / sampleRate_;
        phase_ = 0;
        buffer_.resize(size_ + 1);
        
        for(unsigned int n = 0; n < size_; n++ ){
        buffer_[n] = sin(2 * M_PI * (float)n / (float)size_);
        };
    };
    
    void write(T value){ // Write value to buffer at writepointer index.
        buffer_[writePointer_] = value;
        writePointer_++;
        
    };
    
    void setFrequency(T frequency){ // Write value to buffer at writepointer index
        frequency_ = frequency;
        increment_ = frequency_ * size_ * inverseSampleRate_;
    };
    
    T read(){
        T out;
        
        int indexBelow = floorf(phase_);
        int indexAbove = indexBelow + 1;
        indexAbove &= wrapMask_;
        
        T fractionAbove = phase_ - indexBelow;
        T fractionBelow = 1.0 - fractionAbove;
        
        phase_ += increment_;
        
        if (phase_ >= size_)
            phase_ - size_;
        
        if (phase_ < 0)
            phase_ + size_;
        
        out = (fractionBelow * buffer_[indexBelow]) + (fractionAbove * buffer_[indexAbove]);
        
        return out;
    }
    
    ~WaveTable() {}

private:
    
    std::vector<T> buffer_;
    
    int size_;
    float frequency_;
    int sampleRate_;
    float increment_;
    float inverseSampleRate_;
    int writePointer_;
    int readPointer_;
    int bufferLength_;
    int wrapMask_;
    float phase_;
    int oscDirection;
    
};
}

#endif /* WaveTables_h */


