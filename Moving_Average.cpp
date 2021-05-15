//
//  Moving_Average.cpp
//  Bela Mirror - App
//
//  Created by Adam Pultz Melbye on 11.03.21.
//

#include "Moving_Average.hpp"

using namespace pultzLib;

Moving_Average::Moving_Average(int size){
    initialise(size);
}

void Moving_Average::initialise(int size){
    size_ = (unsigned int) (pow(2, ceil(log(size) / log(2))));
    sizeReciprocal_ = 1.0 / size_;
    buffer.setup(size, 0);
}

float Moving_Average::process(float s){
    float oldestVal = buffer.getOldest();
    buffer.write(s);
    sum += s;
    sum -= oldestVal;
    float average = sum * sizeReciprocal_;
    
    return average;
}
