//
//  Adaptive_Comb.hpp
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 04.07.22.
//

#pragma once

#ifndef Adaptive_Comb_h
#define Adaptive_Comb_h

#include <random>
#include "DelayLin.h"
#include "OnePole_LP.h"
#include "Utilities.hpp"
#include "Fast_Math.hpp"

namespace pultzLib {

class Adaptive_Comb {
    
public:
    Adaptive_Comb(){};
    Adaptive_Comb(float initFreq, float ampDiffThresh, float schmittLow, float schmittHigh, float valSmoothing);
        
    void init(float initFreq, float ampDiffThresh, float schmittLow, float schmittHigh, float valSmoothing);
    
    void setVariables(float freq, float amp, float ampDiff);
    
    float getFbSgn();
    
    void setFbSgn(float fbSgn);
    
    float process(float sig);
    
    ~Adaptive_Comb() {}
    
private:
    
    Schmitt_Trigger schmitt_trigger;
    DelayLin delayLin;
    OnePole_LP fbSignSmooth;
    OnePole_LP ratioSmooth;
    default_random_engine generator;
    
    float ratios_[5] = {1.5, 1.25, 0.875, 1.125, 1.375};
    float delTime_;
    float ampDiffThresh_;
    float freq_;
    float amp_;
    float ampDiff_;
    float fbSgn_ = 1.0;
    bool delTimeShift_ = true;
};

}

#endif /* Adaptive_Comb_h */

