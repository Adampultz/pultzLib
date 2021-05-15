/***** CircularBuffer.h *****/


// Delay line with feedback, linear interpolation and sample-by-sample smoothing ramp

#pragma once

#include <vector>
#include <cmath>

namespace pultzLib {

template<class T>
class CircularBuffer {
public:
	CircularBuffer() {}													// Default constructor
	CircularBuffer(int size, int delay);
			  
	void setup(int size, int delay){
        readIndex_ = delay;
        size_ = size;
        bufferLength_ = (unsigned int) (pow(2, ceil(log(size_) / log(2)))); // Round buffer length to nearest power of 2
        wrapMask_ = bufferLength_ - 1;
        buffer_.resize(bufferLength_);
        writePointer_ = 0;
    };
    
    void setDelay(int delay){ //set readIndex in samples
        readIndex_ = delay;
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
    
    T read(){
        T out;
        int readPointer_ = writePointer_ - readIndex_;
        readPointer_ &= wrapMask_;
        out = buffer_[readPointer_];
        
        return out;
    }
	
	~CircularBuffer() {}				// Destructor

private:
	
	std::vector<T> buffer_;
	int size_;
	int writePointer_;
	int readPointer_;
	int actualReadIndex_;
	int readIndex_;      
	int bufferLength_; // Adjusting the actual buffer to a power of two for masking
	int wrapMask_;
	
};

}
