
#include "Noise_Gate.hpp"

using namespace pultzLib;

Noise_Gate::Noise_Gate(){};

void Noise_Gate::init(float threshold, int filterSize){
    xFade.init(threshold);
    movAverage.init(filterSize);
}

float Noise_Gate::process(float source, float db){
    float filter = movAverage.process(source);
    
    return xFade.process(filter, source, db);    
}
