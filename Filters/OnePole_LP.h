
//  Created by Adam Pultz Melbye on 26.03.21.
//
// Adapted from https://www.earlevel.com/main/2012/12/15/a-one-pole-filter/


#ifndef OnePole_LP_h
#define OnePole_LP_h

#include "Definitions.hpp"
#include <math.h>

using namespace pultzLib;

class OnePole_LP : public Definitions {
public:
    OnePole_LP() {
        a0 = 1.0; b1 = 0.0; z1 = 0.0;
    };
    OnePole_LP(double Fc) {
        z1 = 0.0; setFc(Fc);
    };
    
    void setFc(double Fc);
    float process(float s);
    
    ~OnePole_LP() {};
    
private:
    double a0, b1, z1;
};

inline void OnePole_LP::setFc(double Fc) {
    b1 = exp(-2.0 * M_PI * (Fc / g_SampleRate));
    a0 = 1.0f - b1;
}

inline float OnePole_LP::process(float s) {
    return z1 += (s - z1) * a0;
}


#endif /* OnePole_LP_h */

