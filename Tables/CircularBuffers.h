/***** CircularBuffer.h *****/


// Delay line with feedback, linear interpolation and sample-by-sample smoothing ramp

#pragma once

#include <vector>
#include <cmath>

namespace pultzLib {

template<class T>
class CircularBuffer {
public:
	CircularBuffer() {}	
	CircularBuffer(int size);
			  
	void setup(int size){
        size_ = size;
        bufferLength_ = (unsigned int) (pow(2, ceil(log(size_) / log(2)))); // Round buffer length to nearest power of 2
        wrapMask_ = bufferLength_ - 1;
        buffer_.resize(bufferLength_);
    };
    
    T getOldest(){ // Always call before write
//        int write = writePointer_;
        return buffer_[writePointer_];
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
        int readPointer_ = writePointer_ - readIndex_;
        readPointer_ &= wrapMask_;
        return buffer_[readPointer_];
    }
	
	~CircularBuffer() {}				// Destructor

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

template<class T>
class CircularBufferC {
public:
    CircularBufferC() {}                                                    // Default constructor
    CircularBufferC(int size, int readIndex);
              
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
        int readPointer_ = writePointer_ - readIndex_;
        readPointer_ &= wrapMask_;
        int y_m1_i = readPointer_ - 1;
        y_m1_i &= wrapMask_;
        int y_p1_i = readPointer_ + 1;
        y_p1_i &= wrapMask_;
        int y_p2_i = readPointer_ + 2;
        y_p2_i &= wrapMask_;
        
        float y_ = buffer_[readPointer_];
        float y_m1 = buffer_[y_m1_i];
        float y_p1 = buffer_[y_p1_i];
        float y_p2 = buffer_[y_p2_i];
        
        float ym1py1 = y_m1 + y_p1;
        float c0 = 0.166666666666667 * ym1py1 + 0.666666666666667 * y_;
        float c1 = 0.5 * (y_p1 - y_m1);
        float c2 = 0.5 * ym1py1 - y_;
        float c3 = 0.5 * (y_ - y_p1) + 0.166666666666667 * (y_p2 - y_m1);
        
        return ((c3 * y_ + c2) * y_ + c1) * y_ + c0;
    }
    
    ~CircularBufferC() {}                // Destructor

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

template<class T>
class CircularBufferLin {
public:
    CircularBufferLin() {}                                                    // Default constructor
    CircularBufferLin(int size, int readIndex);
              
    void setup(int size, int readIndex){
        readIndex_ = readIndex;
        size_ = size;
        bufferLength_ = (unsigned int) (pow(2, ceil(log(size_) / log(2)))); // Round buffer length to nearest power of 2
        wrapMask_ = bufferLength_ - 1;
        buffer_.resize(bufferLength_);
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
    
    ~CircularBufferLin() {}                // Destructor

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
