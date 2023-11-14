/*
 Utilities for running pultzLib
 */

#ifndef Utilities_hpp
#define Utilities_hpp

#include "db_lookup_table.hpp"
#include "fundamentals.hpp"
#include "Fast_Math.hpp"
#include "Leaky_Integrator.h"

using namespace pultzLib;

/*
 Adopted from Joel de Guzman's Q library
 Crossfade between two inputs according to a Decibel threshold. If ctrl input falls below the threshold, the crossfade takes effect according to ctrl's distance to the threshold
 */

class level_crossfade
{
public:
    level_crossfade(){};
    
    void init(float thresh){
        thresh_ = thresh;
    };
    
    float process(float a, float b, float ctrl){
        if (ctrl < thresh_)
              {
                 float xfade = ctrl - thresh_;
                  xfade = db2a(xfade);
                 return (xfade * a) + ((1.0f - xfade) * b);
              }
              return a;
    }
    
private:
    float thresh_;
};

/*
 A signal comparator with hysterises
 */

class Schmitt_Trigger
{
public:
    Schmitt_Trigger(){};
    
    void init(float lowerThresh, float upperThresh){
        upperThresh_ = upperThresh;
        lowerThresh_ = lowerThresh;
    };
    
    float process(float s){
        if (s >= upperThresh_ && state_ == 1) {
                 state_ = -1;
              } else if (s < lowerThresh_ && state_ == -1) {
              state_ = 1;
              }
              return state_;
              }
    
private:
    float upperThresh_;
    float lowerThresh_;
    float state_ = 1;
};

template <class T>
class Variance
{
public:
    
    Variance(){}
    
    void init(int size){
    	size_ = size;
        sizeMinOne_ = size_ - 1;
        size_reciproc_ = (float) 1.0 / size_;
        values_.resize(size_);
    };
    
    void push(T val, int index){
        values_[index] += val;
        sum_ += val;
    }
    
    /* Calculate mean and variance */
    float process(){        
        float mean = sum_ * size_reciproc_;
        float runningSum = 0;
        
        for (int n = 0; n < size_; n++){
            runningSum += powf_fast(values_[n] - mean, 2);
        }
        
        variance_ = runningSum * size_reciproc_;
       
        sum_ = 0;
        
        return variance_;
    }
    
    /* If mean is already available, calculate only variance */
    
    float process(float meanVal){
        float mean = meanVal;
        float runningSum = 0;
        
        for (int n = 0; n < size_; n++){
            runningSum += powf_fast(values_[n] - mean, 2); // Calculate the square of the difference between each amplitude and the mean amplitude
            values_[n] = 0;
        }
        
        variance_ = runningSum * size_reciproc_;        
        sum_ = 0;
        
        return variance_;
    }
    
private:
    
    float variance_;
    int size_;
    int sizeMinOne_;
    float size_reciproc_;
    vector<float> values_;
    float sum_;
};

template <class T>
class Zero_Crossing
{
public:
    Zero_Crossing(){};
    
    bool process(T val){
    	unsigned int sign_ = sgn(val * x_m1);
        if (sign_ == 1){
            x_m1 = val;
            return 0;
        } else if (sign_ == -1 || 0) {
            x_m1 = val;
            return 1;
        }
    }
    
private:
    T x_m1;
};

template <class T>
class RunningMax
{
public:
    RunningMax(){}
    
    T process(T x){
        if (x > maxVal_)
            maxVal_ = x;
        
        return maxVal_;
    };
    
    void reset(){
        maxVal_ = 0;
    };
    
    void setMaxVal(T newMaxVal){
        maxVal_ = newMaxVal;
    };
    
private:
    T maxVal_;
};

template <class T>
class RunningMaxDyn
{
public:
    RunningMaxDyn(){}
    RunningMaxDyn(float filterCoeff){
        init(filterCoeff);
    };
    
    void init(float filterCoeff){
        leaky_Integrator.init(filterCoeff);
    };
    
    T process(T x){
        float oneShot = 0;
        
        if (x > maxVal_){
            leaky_Integrator.reset();
            oneShot = x;
        };
        
        maxVal_ = leaky_Integrator.process(oneShot);
        
        return maxVal_;
    };
    
    void reset(){
        maxVal_ = 0;
    };
    
    void setMaxVal(T newMaxVal){
        maxVal_ = newMaxVal;
    };
    
private:
    T maxVal_;
    Leaky_Integrator leaky_Integrator;
};


template <class T>
class RunningMin
{
public:
    RunningMin(){};
    
    void init(T min){
        minVal_ = min;
    };
    
    T process(T x){
        if (x < minVal_)
            minVal_ = x;
        
        return minVal_;
    };
    
    void reset(){
        minVal_ = 0;
    };
    
    void setMinVal(T newMinVal){
        minVal_ = newMinVal;
    };
    
private:
    T minVal_;
};

/* For crossfading between two values. Using a square root function to preserve amplitude*/
static inline float Balance2(float val_a, float val_b, float balance) {
	return (val_a * sqrtf(1.0f - balance)) + (val_b * sqrtf(balance));
}


#endif /* Utilities_hpp */

