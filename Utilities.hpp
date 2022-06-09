//
//  Utilities.hpp
//  Bela Mirror
//
//  Created by Adam Pultz Melbye on 09.03.21.
//

#ifndef Utilities_hpp
#define Utilities_hpp

#include "db_lookup_table.hpp"
#include "fundamentals.hpp"

using namespace pultzLib;

class level_crossfade
{
public:
    level_crossfade(){};
    
    void initialise(float thresh){
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

class Schmitt_Trigger
{
public:
    Schmitt_Trigger(){};
    
    void init(float upperThresh, float lowerThresh){
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
    int state_ = 1;
};

template <class T>
class Variance
{
public:
    
    Variance(int size){
    	size_ = size;
        sizeMinOne_ = size_ - 1;
        size_reciproc_ = (float) 1 / size_;
        values_.resize(size_);
    };
    
    void push(T val, int index){
        values_[index] = val;
        sum_ += val;
    }
    
    float process(){ // Calculate mean and variance
        
        float mean = sum_ * size_reciproc_;
        float runningSum = 0;
        
        for (int n = 0; n < size_; n++){
            runningSum += powf(values_[n] - mean, 2);
        }
        
        variance_ = runningSum * size_reciproc_;
       
        sum_ = 0;
        
        return variance_;
    }
    
    float process(float meanVal){ //If mean is already available, calculate only variance
        float mean = meanVal;
        float runningSum = 0;
        
        for (int n = 0; n < size_; n++){
            runningSum += powf(values_[n] - mean, 2);
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

static inline float Balance2(float val_a, float val_b, float balance) {
	return (val_a * sqrtf(1.0f - balance)) + (val_b * sqrtf(balance));
}


#endif /* Utilities_hpp */

