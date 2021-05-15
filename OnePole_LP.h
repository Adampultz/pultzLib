
//  Created by Adam Pultz Melbye on 26.03.21.
//
// Adapted from https://www.earlevel.com/main/2012/12/15/a-one-pole-filter/


#ifndef OnePole_LP_h
#define OnePole_LP_h

#include <math.h>

class OnePole_LP {
public:
    OnePole_LP() {
        a0 = 1.0; b1 = 0.0; z1 = 0.0;};
    OnePole_LP(double Fc, float sampleRate) {
        z1 = 0.0; setFc(Fc, sampleRate);};
    
    void setFc(double Fc, float sampleRate);
    float process(float s);
    
    ~OnePole_LP() {};
    
private:
    double a0, b1, z1;
};

inline void OnePole_LP::setFc(double Fc, float sampleRate) {
    double invSRate = 1.0 / sampleRate;
    b1 = exp(-2.0 * M_PI * (Fc * invSRate));
    a0 = 1.0 - b1;
}

inline float OnePole_LP::process(float s) {
    return z1 = s * a0 + z1 * b1;
}




#endif /* OnePole_LP_h */
