/** CircularBuffer.h
Buffers
 ****/

#pragma once

#include <vector>
#include <cmath>
#include "fundamentals.hpp"

namespace pultzLib {

/* Buffer with no interpolation */

template<class T>
class CircularBuffer {
public:
	CircularBuffer() {}	
	CircularBuffer(int size, int readIndex);
    CircularBuffer(int size);
			  
	void init(int size, int readIndex){
        size_ = size;
        readIndex_ = readIndex;
        unsigned int bufferLength = nextPow2(size); // Round buffer length to nearest power of 2
        wrapMask_ = bufferLength - 1;
        buffer_.resize(bufferLength);        
    };
    
    void init(int size){
        size_ = size;
        unsigned int bufferLength = nextPow2(size); // Round buffer length to nearest power of 2
        wrapMask_ = bufferLength - 1;
        buffer_.resize(bufferLength);
    };
    
    // For defining a read and write length shorter than the actual buffer length
    void setStrictSize(int writeWindow){
        strictSize_ = writeWindow;
    }
    
    T getOldest(){ // Always call before write
        return buffer_[writePointer_];
    };
    
    void setReadIndex(float value){ //set readIndex in samples
        readIndex_ = value;
    };
	
	void writeStrict(T value){ // Write value to buffer at writepointer index.
        buffer_[writePointer_] = value;
        if (writePointer_++ >= strictSize_)
            writePointer_ = 0; 
    };
    
    void writePow2(T value){ // Write value to buffer at writepointer index.
        buffer_[writePointer_] = value;
        writePointer_++;
        writePointer_ &= wrapMask_; // Wrap around bitwise
    };
    
    void resetWritePointer(){
        writePointer_ = 0;
    };
    
    int getWritepointer(){
        return writePointer_;
    };

    T readPow2(){
        int readPointer_ = writePointer_ - readIndex_;
        readPointer_ &= wrapMask_;
        
        return buffer_[readPointer_];
    }
    
    T readStrict(){
        int readPointer_ = writePointer_ - readIndex_;
        if (readPointer_ >= strictSize_)
            readPointer_ = 0;
        
        return buffer_[readPointer_];
    }
	
    T read(int index){
        int readPointer_ = index;
        float out = buffer_[readPointer_];
        
        return out;
    }
    
	~CircularBuffer() {}

private:
	
	std::vector<T> buffer_;
	unsigned int size_;
    unsigned int strictSize_;
	unsigned int writePointer_;
	unsigned int readIndex_;
	unsigned int wrapMask_;
	
};

/* Buffer with cubic interpolation */


template<class T>
class CircularBufferC : public CircularBuffer<T> {
public:
    CircularBufferC() {}
    CircularBufferC(int size, int readIndex);
    
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
    
    ~CircularBufferC() {}

private:
    
    std::vector<T> buffer_;
    int size_;
    int writePointer_;
    int readIndex_;
    int wrapMask_;
};

/* Buffer with linear interpolation */

template<class T>
class CircularBufferLin : public CircularBuffer<T> {
public:
    CircularBufferLin() {}
    CircularBufferLin(int size, int readIndex);
    
    T read(){
        T out;
        
        int readPointer = writePointer_ - readIndex_;
        readPointer &= wrapMask_;
        int indexBelow = floorf(readPointer);
        int indexAbove = indexBelow + 1;
        indexAbove &= wrapMask_;
        float fractionAbove = readPointer - indexBelow;
        float fractionBelow = 1 - fractionAbove;
        
        out = (fractionBelow * buffer_[indexBelow]) + (fractionAbove * buffer_[indexAbove]);
        
        return out;
    }
    
    ~CircularBufferLin() {}

private:
    
    std::vector<T> buffer_;
    int size_;
    int writePointer_;
    int readIndex_;
    int wrapMask_;
};

}
