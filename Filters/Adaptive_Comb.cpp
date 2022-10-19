//
//  adaptiveComb.hpp
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 04.07.22.
//
 
#include "Adaptive_Comb.h"

using namespace pultzLib;

Adaptive_Comb::Adaptive_Comb(float initFreq, float ampDiffThresh, float schmittLow, float schmittHigh, float valSmoothing, float sampleRate){
    init(initFreq, ampDiffThresh, schmittLow, schmittHigh, valSmoothing, sampleRate);
    }
    
    void Adaptive_Comb::init(float initFreq, float ampDiffThresh, float schmittLow, float schmittHigh, float valSmoothing, float sampleRate){
        delTime_ = 1.0 / initFreq;
        sampleRate_ = sampleRate;
        ampDiffThresh_ = ampDiffThresh;
        ratioSmooth.setFc(valSmoothing, sampleRate_);
        fbSignSmooth.setFc(10, sampleRate_);
        schmitt_trigger.init(schmittLow, schmittHigh);
        delayLin.setup(0.8, delTime_, 0.8, sampleRate_);
        freq_ = 440;
    }
    
    void Adaptive_Comb::setVariables(float freq, float amp, float ampDiff){
        freq_ = freq;
        amp_ = amp;
        ampDiff_ = ampDiff;
        
        // fbSgn_ = schmitt_trigger.process(amp);
        // fbSgn_ = fbSignSmooth.process(fbSgn_);
     //   fbSgn_ = clamp2((float)fbSgn_, -1.0f, 1.0f);
        delayLin.setFeedback(clamp2(float(0.9f - powf_fast(ampDiff_, 2)), 0.0f, 0.9f ) * fbSgn_);
                // delayLin.setFeedback(clamp2(float(0.9f), 0.0f, 0.9f ) * fbSgn_);

        // delayLin.setFeedback(clamp2(float (0.9f - ampDiff), 0.0f, 0.9f ));

    }
    
    float Adaptive_Comb::getFbSgn(){
        return fbSgn_;
    }
    
    void Adaptive_Comb::setFbSgn(float fbSgn){
        fbSgn_ = fbSgn;
    }
    
    float Adaptive_Comb::process(float sig){
        
        if (delTimeShift_ == false && ampDiff_ < ampDiffThresh_)
            delTimeShift_ = true;
       
        if (delTimeShift_ == true && ampDiff_ >= ampDiffThresh_){
        uniform_int_distribution<int> distribution(0, 4);
        int ratioChoice = distribution(generator); // Random integer between 0 and numRatios - 1
        float ratio = ratios_[ratioChoice]; // Float value of ratio
        delTime_ = 1.0f / (freq_ * ratio); // Delay time (inverse of estimated frequency times ratio)
        // delTime_ = 1.0f / (freq_);
        // delTimeShift_ = false;
        }
        
        delTime_ = ratioSmooth.process(delTime_);
        delTime_ =  clamp2(delTime_, 0.00004f, 0.03f);
        delayLin.setDelayTime(delTime_); // = 1.0 / ratio / zc

        
        return delayLin.process(sig);
        
    }
    



