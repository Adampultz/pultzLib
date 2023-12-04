//
//  Moving_Average.cpp
//  Bela Mirror - App
//
//  Created by Adam Pultz Melbye on 11.03.21.
//

#include "Moving_Average.hpp"

using namespace pultzLib;

Moving_Average::Moving_Average(int size){
    init(size);
}

void Moving_Average::init(int size){
    size_ = size;
    sizeReciprocal_ = 1.0 / size_;
    buffer.init(size_);
}

float Moving_Average::process(float s){
    float oldestVal = buffer.getOldest();
    buffer.writePow2(s);
    sum += s;
    sum -= oldestVal;
    float average = sum * sizeReciprocal_;
    
    return average;
}
