/***** RingBuffer.cpp *****/

#include "RingBuffer.h"
#include <cmath>


// Constructor taking arguments for sample rate and table data
RingBuffer::RingBuffer(unsigned int size) {
	setup(size);
}

void RingBuffer::setup(unsigned int size)
{
	array_.resize(size); // Resize the array
	array_ = {}; // Populate the array with zeros
	size_ = size;
}

unsigned int RingBuffer::getSize()
{
	return size_;
}


// Set the value at current index 
void RingBuffer::setVal(float value, unsigned int index) {
array_[index] = value_;

readPointer_ ++;
while (readPointer_ > size_)
readPointer_ -= size_;
}
