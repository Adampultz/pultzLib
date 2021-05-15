/***** RingBuffer.h *****/

#pragma once

#include <vector>

class RingBuffer {
	
public:

	RingBuffer() {}											// Default constructor
	RingBuffer(unsigned int size);
			  
	void setup(unsigned int size); 	
	
	void resize(unsigned int size); // Resize the buffer
	
	void setVal(float value, unsigned int index);
	
	float getVal(unsigned int index);
	
	unsigned int getSize(); // Get the size of the buffer
	
	~RingBuffer() {}				// Destructor

private:
	
	std::vector<float> array_;
	unsigned int size_;
	float value_;
	unsigned int index_;
	unsigned int readPointer_;
};
