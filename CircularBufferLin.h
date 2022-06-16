// Delay line with feedback, linear interpolation and sample-by-sample smoothing ramp

#pragma once

#include <vector>
#include <cmath>

namespace pultzLib {

template<class T>
class CircularBufferLin {
public:
	CircularBufferLin() {}													// Default constructor
	CircularBufferLin(int size, int readIndex);
			  
	void setup(int size, int readIndex){
        readIndex_ = readIndex;
        size_ = size;
        bufferLength_ = (unsigned int) (pow(2, ceil(log(size_) / log(2)))); // Round buffer length to nearest power of 2
        wrapMask_ = bufferLength_ - 1;
        buffer_.resize(bufferLength_ + 1);
        writePointer_ = 0;
    };
    
    void setReadIndex(float value){ //set readIndex in samples
        readIndex_ = value;
    };
    
    T getOldest(){ // Always call before write
        T out;
        int pointer = writePointer_;
        out = buffer_[pointer];
        return out;
    };
	
	void write(T value){ // Write value to buffer at writepointer index.
        buffer_[writePointer_] = value;
        writePointer_++;
        writePointer_ &= wrapMask_; // Wrap around bitwise
    };
    
    int getWritepointer(){
        return writePointer_;
    };
    
    T read(){
        T out;
        
        int readPointer_ = writePointer_ - readIndex_;
        readPointer_ &= wrapMask_;
        int indexBelow = floorf(readPointer_);
        int indexAbove = indexBelow + 1;
        indexAbove &= wrapMask_;
        float fractionAbove = readPointer_ - indexBelow;
        float fractionBelow = 1 - fractionAbove;
        
        out = (fractionBelow * buffer_[indexBelow]) + (fractionAbove * buffer_[indexAbove]); // Linear interpolation. To Do: Lagrange
        
        return out;
    }
	
	~CircularBufferLin() {}				// Destructor

private:
	
	std::vector<T> buffer_;
	int size_;
	int writePointer_;
	int readPointer_;
	int actualReadIndex_;
	int readIndex_;          // Samplerate
	int bufferLength_; // Adjusting the actual buffer to a power of two for masking
	int wrapMask_;
};

}
