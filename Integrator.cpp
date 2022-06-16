#include "Integrator.h"

Integrator::Integrator(float rate, float sampleRate){
    setup(rate = 15.0, sampleRate = 48000.0);
}

void Integrator::setup(float rate, float sampleRate){
    rate_ = 1.0 / rate;
    sampleRate_ = sampleRate;
    inverseSampleRate_ = 1.0 / sampleRate;
}

float Integrator::process(float val){
    float y = val * rate_ + y_n_m1;
    if(std::isnan(y) == true)
        y = 0.0;
    y_n_m1 = y;
    
    return y;
}
