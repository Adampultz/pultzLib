#ifndef Integrator_h
#define Integrator_h

#include <stdio.h>
#include <cmath>

class Integrator
{
public:
    Integrator(){}
    Integrator(float rate, float sampleRate);
    
    void setup(float rate, float sampleRate);
    
    ~Integrator() {}
    
   float process(float in);
    
private:
    float y_n_m1 = 0.0;
    float rate_;
    float sampleRate_;
    float inverseSampleRate_;
};

#endif /* Integrator_h */
